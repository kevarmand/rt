/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 21:55:31 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 01:59:20 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDER_H
# define RENDER_H

# include <stdint.h>
# include "pthread.h"
# include <stdatomic.h>
# include "rt_config.h"
# include "vector.h"
# include "scene.h"
# include "postprocess.h"

typedef struct s_data	t_data;

typedef struct s_render_view
{
	t_vec3f	origin;
	t_vec3f	forward;
	t_vec3f	right;
	t_vec3f	up;
	t_vec3f	p0;
	t_vec3f	dx;
	t_vec3f	dy;
	float	fov_deg;
	int		frame_seq;
	int		mode;
	int		ssaa;
}	t_render_view;

typedef struct s_tile
{
	int		tile_id;
	int		pixel_x;
	int		pixel_y;
	int		pixel_width;
	int		pixel_height;
	t_vec3f	*hdr_pixels;
}	t_tile;

typedef enum e_worker_state
{
	WORKER_IDLE,
	WORKER_BUSY,
	WORKER_DONE
}	t_worker_state;

typedef struct s_worker
{
	pthread_t		thread_id;
	t_tile			tile;
	int				camera_id;
	t_render_view	local_view;
	atomic_int		worker_state;
}	t_worker;

typedef struct s_workers
{
	t_worker	*array;
	int			count;
}	t_workers;

typedef struct s_tileset
{
	int			tile_width;
	int			tile_height;
	int			tiles_per_row;
	int			tiles_per_col;
	int			tiles_total;
	int			assigned;
	int			step;
	int			*cursor;
}	t_tileset;

typedef struct s_display_mailbox
{
	t_camera	cam;
	int			request_camera_id;
	int			req_job_id;
	int			request_render_mode;
	int			request_ssaa;
	atomic_int	request_ready;

	atomic_uint	post_settings;
	atomic_int	active_cam;
	atomic_int	*expected_seq;
	/* Writable spare when snapshot_ready == 0, offered image when 1. */
	int			image_id;
	int			tile_count;
	int			tiles_done;
	int			snap_camera_id;
	int			snap_job_id;
	int			snapshot_render_mode;
	unsigned int	snap_post_settings;
	int				snap_auto_ev;
	atomic_int		snapshot_ready;
}	t_display_mailbox;

typedef struct s_hdr_frame
{
	t_vec3f			*pixels;
	int				*tile_seq;
	int				cursor;
	int				job_id;
	int				mode;
	int				done;
	int				auto_ev;
	uint64_t		revision;
	uint64_t		published_revision;
	unsigned int	published_settings;
}	t_hdr_frame;

typedef struct s_rgb_image
{
	int				*pixels;
	int				*tile_seq;
	int				camera_id;
	unsigned int	settings;
}	t_rgb_image;

/* Metadata is frozen at pass start. Later HDR writes schedule another pass. */
typedef struct s_rgb_pass
{
	int				camera_id;
	int				next_tile;
	int				job_id;
	int				mode;
	int				done;
	int				auto_ev;
	uint64_t		revision;
}	t_rgb_pass;

typedef struct s_mgr
{
	pthread_t		thread_id;
	int				render_in_progress;
	int				current_cam_id;
	t_tileset		tileset;
	t_render_view	render_view;
	float			*tonemap_exposure;
	t_hdr_frame		*frames;
	int				camera_count;
	float			*gamma_lut;
	int				lut_gamma;
	unsigned int	post_settings;
	int				active_cam;
	t_rgb_image		*images;
	t_rgb_pass		pass;
}	t_mgr;

typedef struct s_render
{
	int					width;
	int					height;

	t_workers			workers;
	t_mgr				manager;
	t_display_mailbox	mailbox;

	int					manager_started;
	int					workers_count;
	atomic_int			cancel_flag;
}	t_render;

/***
 * Initialize the render structure
 * return SUCCESS if ok, error code otherwise
 * @param render pointer to the render structure
 * @param width width of the render
 * @param height height of the render
 * @return 0 on SUCCESS, error code otherwise
 */
int		render_init(t_render *render, int width, int height, int cameras);

/* ************************************************************************** */
/*         					     RENDER FUNCTIONS                            */
/* ************************************************************************** */

/***
 * Manager thread function
 * @param arg pointer to the render structure
 * @return NULL
 */
void	*manager_entry(void *arg);

/***
 * Worker thread function
 * @param arg pointer to the worker structure
 * @return NULL
 */
void	*worker_entry(void *arg);

/* ************************************************************************** */
/*         					     MANAGER FUNCTIONS                            */
/* ************************************************************************** */

/***
 * Dispatch a tile to a worker
 * @param tileset pointer to the tileset structure
 * @return tile id, or -1 if no more tiles available
 */
int		manager_dispatch_tile(t_tileset *tileset);

int		init_worker_tile(t_tileset *tileset, t_tile *tile,
			int image_width, int image_height);

int		tile_permutation_step(int tile_count);

/* view */
void	view_setup(t_render_view *view, int width, int height);

/* render tile */
int		render_tile_normal(t_data *data, t_tile *tile,
			const t_render_view *view);

int		render_tile_fast(t_data *data, t_tile *tile,
			const t_render_view *view);

int		render_tile_super(t_data *data, t_tile *tile,
			const t_render_view *view);

#endif
