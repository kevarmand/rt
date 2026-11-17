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

static int	render_init_main_buffers(t_render *render, int cameras)
{
	t_mgr	*mgr;
	size_t	pixels;
	int		i;

	mgr = &render->manager;
	pixels = (size_t)render->width * render->height;
	if (render->width <= 0 || render->height <= 0 || cameras <= 0
		|| pixels > SIZE_MAX / sizeof(t_vec3f))
		return (ERR_INTERNAL);
	mgr->hdr_buffer = ft_calloc(pixels, sizeof(t_vec3f));
	mgr->frames = ft_calloc((size_t)cameras, sizeof(t_hdr_frame));
	mgr->tonemap_exposure = malloc(sizeof(float) * TONESIZE);
	mgr->gamma_lut = malloc(sizeof(float) * POST_LUT_SIZE);
	mgr->lut_gamma = -1;
	mgr->post_settings = post_default();
	if (!mgr->hdr_buffer || !mgr->frames || !mgr->tonemap_exposure
		|| !mgr->gamma_lut)
		return (ERR_MALLOC);
	mgr->camera_count = cameras;
	i = 0;
	while (i < cameras)
	{
		mgr->frames[i].pixels = ft_calloc(pixels, sizeof(t_vec3f));
		if (!mgr->frames[i].pixels)
			return (ERR_MALLOC);
		mgr->frames[i].auto_exposure = 1.0f;
		i++;
	}
	return (SUCCESS);
}

/* ---------- Helper 3 : init mailbox buffers ---------- */

static int	render_init_mailbox(t_render *render)
{
	int		total_pixels;
	int		i;
	size_t	bytes_rgb;

	total_pixels = render->width * render->height;
	bytes_rgb = (size_t)total_pixels * sizeof(int);
	render->mailbox.rgb_pixels = malloc(bytes_rgb);
	render->mailbox.expected_seq = malloc(sizeof(atomic_int)
			* (size_t)render->manager.camera_count);
	if (!render->mailbox.rgb_pixels || !render->mailbox.expected_seq)
		return (ERR_MALLOC);
	i = 0;
	while (i < render->manager.camera_count)
	{
		atomic_init(&render->mailbox.expected_seq[i], 0);
		i++;
	}
	ft_memset(render->mailbox.rgb_pixels, 0, bytes_rgb);
	atomic_store(&render->mailbox.snapshot_ready, 0);
	atomic_store(&render->mailbox.request_ready, 0);
	atomic_init(&render->mailbox.post_settings, post_default());
	atomic_init(&render->mailbox.active_cam, 0);
	return (SUCCESS);
}

int	render_init(t_render *render, int width, int height, int cameras)
{
	int	status;

	render_init_struct(render, width, height);
	status = render_init_main_buffers(render, cameras);
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
