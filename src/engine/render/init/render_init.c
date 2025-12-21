/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 17:46:18 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 07:21:30 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include <stdlib.h>
#include "errors.h"
#include "engine.h"
#include "vector.h"
#include "rt_config.h"
#include "bitmap.h"
#include "libft.h"
#include <stdatomic.h>

/* ---------- Forward declarations ---------- */
int	render_init_workers(t_render *render);
int	render_init_worker_tiles(t_render *render);
int	render_init_tileset(t_render *render);

/* ---------- Helper 1 : initialize render struct ---------- */

static void	render_init_struct(t_render *render, int width, int height)
{
	ft_memset(render, 0, sizeof(t_render));
	render->width = width;
	render->height = height;
	atomic_store(&render->cancel_flag, 0);
}

/* ---------- Helper 2 : allocate HDR + RGB main buffers ---------- */

static int	render_init_main_buffers(t_render *render)
{
	int		total_pixels;
	size_t	bytes_hdr;
	size_t	bytes_rgb;

	total_pixels = render->width * render->height;
	if (total_pixels <= 0)
		return (ERR_INTERNAL);
	bytes_hdr = (size_t)total_pixels * sizeof(t_vec3f);
	bytes_rgb = (size_t)total_pixels * sizeof(int);
	render->manager.hdr_buffer = malloc(bytes_hdr);
	if (!render->manager.hdr_buffer)
		return (ERR_MALLOC);
	ft_memset(render->manager.hdr_buffer, 0, bytes_hdr);
	render->manager.rgb_buffer = malloc(bytes_rgb);
	if (!render->manager.rgb_buffer)
		return (ERR_MALLOC);
	ft_memset(render->manager.rgb_buffer, 0, bytes_rgb);
	render->manager.tonemap_exposure = malloc(sizeof(float) * TONESIZE);
	if (!render->manager.tonemap_exposure)
		return (ERR_MALLOC);
	return (SUCCESS);
}

/* ---------- Helper 3 : init mailbox buffers ---------- */

static int	render_init_mailbox(t_render *render)
{
	int		total_pixels;
	size_t	bytes_rgb;

	total_pixels = render->width * render->height;
	bytes_rgb = (size_t)total_pixels * sizeof(int);
	render->mailbox.rgb_pixels = malloc(bytes_rgb);
	if (!render->mailbox.rgb_pixels)
		return (ERR_MALLOC);
	ft_memset(render->mailbox.rgb_pixels, 0, bytes_rgb);
	atomic_store(&render->mailbox.snapshot_ready, 0);
	atomic_store(&render->mailbox.request_ready, 0);
	return (SUCCESS);
}

int	render_init(t_render *render, int width, int height)
{
	int	status;

	render_init_struct(render, width, height);
	status = render_init_main_buffers(render);
	if (status != SUCCESS)
		return (perr(status, PERR_M_MAIN_BUFFERS));
	status = render_init_mailbox(render);
	if (status != SUCCESS)
		return (perr(status, PERR_M_MAILBOX));
	status = render_init_workers(render);
	if (status != SUCCESS)
		return (perr(status, PERR_M_WORKERS));
	status = render_init_worker_tiles(render);
	if (status != SUCCESS)
		return (perr(status, PERR_M_WORKER_TILES));
	status = render_init_tileset(render);
	if (status != SUCCESS)
		return (perr(status, PERR_M_TILESET));
	render->manager.render_in_progress = 0;
	render->manager.thread_id = 0;
	return (SUCCESS);
}
