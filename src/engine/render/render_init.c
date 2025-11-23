/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 17:46:18 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/23 18:12:10 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include <stdlib.h>
#include <string.h>
#include "errors.h"
#include "engine.h"
#include "vector.h"
#include "rt_config.h"


static int	round_up_div(int n, int tile)
{
	return ((n + tile - 1) / tile);
}

static int	render_compute_worker_count(void)
{
	if (MAX_WORKER_THREADS < 1)
		return (0);
	return (MAX_WORKER_THREADS);
}

static void	render_set_defaults(t_render *render, const t_engine *engine)
{
	render->width = engine->width;
	render->height = engine->height;
	render->workers = NULL;
	render->worker_count = render_compute_worker_count();
	render->tiles_bitmap = NULL;
	render->tiles_total = 0;
	atomic_store(&render->tiles_dispatched, 0);
	atomic_store(&render->tiles_done, 0);
	render->rgb_front = NULL;
	render->rgb_back = NULL;
	atomic_store(&render->rgb_ready, 0);
	atomic_store(&render->rgb_busy, 0);
	render->manager_created = 0;
	atomic_store(&render->cancel_flag, 0);
}

static int	init_one_worker(t_worker *worker)
{
	size_t	count;
	size_t	bytes;

	worker->thread_id = 0;
	atomic_store(&worker->has_job, 0);
	worker->tile.id = -1;
	worker->tile.x = 0;
	worker->tile.y = 0;
	atomic_store(&worker->tile.is_done, 1);
	count = (size_t)TILE_SIZE * (size_t)TILE_SIZE;
	bytes = count * sizeof(t_vec3f);
	worker->tile.hdr_pixels = malloc(bytes);
	if (!worker->tile.hdr_pixels)
		return (ERR_MALLOC);
	memset(worker->tile.hdr_pixels, 0, bytes);
	return (SUCCESS);
}

static int	render_init_workers(t_render *render)
{
	int		i;
	int		status;

	if (render->worker_count <= 0)
		return (SUCCESS);
	render->workers = malloc(sizeof(t_worker) * render->worker_count);
	memset(render->workers, 0, sizeof(t_worker) * render->worker_count);
	if (!render->workers)
		return (ERR_MALLOC);
	i = 0;
	while (i < render->worker_count)
	{
		status = init_one_worker(&render->workers[i]);
		if (status != SUCCESS)
			return (status);
		i++;
	}
	return (SUCCESS);
}


static int	render_init_tiles_bitmap(t_render *render)
{
	int			gw;
	int			gh;
	int			words;
	size_t		bytes;

	gw = round_up_div(render->width, TILE_SIZE);
	gh = round_up_div(render->height, TILE_SIZE);
	render->tiles_total = gw * gh;
	if (render->tiles_total <= 0)
		return (ERR_INTERNAL);
	words = (render->tiles_total + 63) / 64;
	bytes = (size_t)words * sizeof(uint64_t);
	render->tiles_bitmap = malloc(bytes);
	if (!render->tiles_bitmap)
		return (ERR_MALLOC);
	memset(render->tiles_bitmap, 0, bytes);
	atomic_store(&render->tiles_dispatched, 0);
	atomic_store(&render->tiles_done, 0);
	return (SUCCESS);
}

int	render_init_threads(t_render *render, void *thread_arg)
{
	int		worker_index;
	int		status;

	render->workers_created = 0;
	render->manager_created = 0;
	atomic_store(&render->cancel_flag, 0);

	if (render->worker_count <= 0)
		return (SUCCESS);

	worker_index = 0;
	while (worker_index < render->worker_count)
	{
		status = pthread_create(&render->workers[worker_index].thread_id,
				NULL, worker_thread, thread_arg);
		if (status != 0)
			return (ERR_THREAD);
		render->workers_created++;
		worker_index++;
	}
	status = pthread_create(&render->manager_thread,
			NULL, manager_thread, thread_arg);
	if (status != 0)
		return (ERR_THREAD);
	render->manager_created = 1;
	return (SUCCESS);
}


static int	render_init_rgb_buffers(t_render *render)
{
	int		pixels;
	size_t	bytes;

	pixels = render->width * render->height;
	if (pixels <= 0)
		return (ERR_INTERNAL);
	bytes = (size_t)pixels * 3 * sizeof(int);
	render->rgb_front = malloc(bytes);
	if (!render->rgb_front)
		return (ERR_MALLOC);
	render->rgb_back = malloc(bytes);
	if (!render->rgb_back)
		return (ERR_MALLOC);
	memset(render->rgb_front, 0, bytes);
	memset(render->rgb_back, 0, bytes);
	atomic_store(&render->rgb_ready, 0);
	atomic_store(&render->rgb_busy, 0);
	return (SUCCESS);
}




int	render_init(t_render *render, const t_engine *engine)
{
	int	status;

	memset(render, 0, sizeof(t_render));
	render_set_defaults(render, engine);
	status = render_init_workers(render);
	if (status != SUCCESS)
		return (status);
	status = render_init_tiles_bitmap(render);
	if (status != SUCCESS)
		return (status);
	status = render_init_rgb_buffers(render);
	if (status != SUCCESS)
		return (status);
	status = render_init_threads(render, (void *)engine);
    return (status);
}
