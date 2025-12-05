#ifndef RENDER_H
# define RENDER_H

# include <stdint.h>
# include "pthread.h"
# include <stdatomic.h>
# include "rt_config.h"
# include "vector.h"
# include "scene.h"
# include "bitmap.h"

typedef struct s_data t_data;

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
	int			tiles_active;
	int			tiles_ready;
	int			tiles_done;

	t_bitmap	tile_state;	
}	t_tileset;

typedef struct s_display_mailbox
{
	int			*rgb_pixels;
	int			tile_count;
	int			tiles_done;
	atomic_int	snapshot_ready;

	t_camera	cam;
	atomic_int	request_ready;
}	t_display_mailbox;

typedef struct s_mgr
{
	pthread_t		thread_id;
	int				render_in_progress;

	t_tileset		tileset;
	int				*rgb_buffer;
	t_vec3f			*hdr_buffer;
	t_render_view	render_view;
}	t_mgr;

typedef struct s_display_thread
{
	int			*front_pixels;
	t_camera	current_cam;
}	t_display_thread;


typedef struct s_render
{
	int					width;
	int					height;

	t_workers			workers;     // pool de workers
	t_mgr				manager;     // thread "Karen"
	t_display_mailbox	mailbox;     // partagé manager <-> mlx_thread
	atomic_int			cancel_flag;  // ou stop_requested

}	t_render;



/***
 * Initialize the render structure
 * return SUCCESS if ok, error code otherwise
 * @param render pointer to the render structure
 * @param width width of the render
 * @param height height of the render
 * @return 0 on SUCCESS, error code otherwise
 */
int		render_init(t_render *render, int width, int height);



/* ************************************************************************** */
/*         					     RENDER FUNCTIONS                            */
/* ************************************************************************** */

/***
 * Manager thread function
 * @param arg pointer to the render structure
 * @return NULL
 */
void *manager_thread(void *arg);


/***
 * Worker thread function
 * @param arg pointer to the worker structure
 * @return NULL
 */
void *worker_thread(void *arg);


/* ************************************************************************** */
/*         					     MANAGER FUNCTIONS                              */
/* ************************************************************************** */

/***
 * Dispatch a tile to a worker
 * @param tileset pointer to the tileset structure
 * @return tile id, or -1 if no more tiles available
 */
int		manager_dispatch_tile(t_tileset *tileset);

int init_worker_tile(t_tileset *tileset, t_tile *tile,
			int image_width, int image_height);

/* dispatch */
int  manager_dispatch_tile(t_tileset *tileset);

/* worker tile */
int  init_worker_tile(t_tileset *tileset, t_tile *tile,
                      int image_width, int image_height);

/* threads */
void *worker_thread(void *arg);
void *manager_thread(void *arg);

/* manager core */
int  manager_collect_tiles(t_render *render);
int  manager_assign_jobs(t_render *render);
int  manager_convert_hdr_to_rgb(t_render *render);
int  manager_update_display(t_render *render);
int  manager_read_mailbox(t_render *render);
void manager_run_tonemap(t_render *render);

/* view */
void view_setup(t_render_view *view, int width, int height);

/* render tile */
int  render_tile(t_data *data, t_tile *tile, const t_render_view *view);

/* init */
int  render_init(t_render *render, int width, int height);

#endif