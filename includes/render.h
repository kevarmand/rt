#ifndef RENDER_H
# define RENDER_H

# include <stdint.h>
# include "pthread.h"
# include <stdatomic.h>
# include "rt_config.h"

typedef struct s_data t_data;

typedef struct s_tile
{
	int			id;
	int			x;
	int			y;
	t_vec3f		*hdr_pixels;
	t_cam_view	*cam_view;
	atomic_int	is_done;
}	t_tile;

typedef struct s_worker
{
	pthread_t	thread_id;
	t_tile		tile;
	atomic_int	has_job;
	t_data		*data;
}	t_worker;


typedef struct s_render
{
	int			width;
	int			height;

	t_worker	*workers;
	int			worker_count;
	int			workers_created;

	/* bitmap des tiles globales à faire (id, done, etc.) */
	uint64_t	*tiles_bitmap;
	int			tiles_total;
	atomic_int	tiles_dispatched;
	atomic_int	tiles_done;

	int			*rgb_front;
	int			*rgb_back;
	atomic_int	rgb_ready;
	atomic_int	rgb_busy;

	pthread_t	manager_thread;
	int			manager_created;
	atomic_int	cancel_flag;
}	t_render;





/***
 * comment in english
 * Inititialize the render structure 
 * Allocate the hdr buffer
 * Allocate the tile buffer
 * Allocate the done_bitmap buffer
 * Initialize the tile buffer to 0
 * Initialize the done_bitmap buffer to 0
 *  Return SUCCESS or ERROR
 */
int		ren_init(t_data *data);


/***
 * 
 * Initialize the tiles
 * Allocate the tile buffer for each tile
 * Initialize the id to -1
 * Initialize the tile_buffer to 0
 * Return SUCCESS or ERROR
 */
int	init_tiles(t_data *data);

/* cleanup functions */
void	tile_free(t_tile *tile);
void	ren_tiles_free(t_data *data);
void	ren_free(t_data *data);
void	ren_destroy(t_data *data);

/***
 * Reset the done_bitmap and tile_done to 0
 * @param done_bitmap: pointer to the done_bitmap array
 * @param tile_total: total number of tiles
 * @param tile_done: pointer to the tile_done counter
 * 
 * @return void
 */
void	reset_tile(uint64_t *done_bitmap, int tile_total, int *tile_done);

/***
 * Return a pseudo-random unfinished tile index and mark it as done.
 * @param done_bitmap: bitfield marking done tiles (size = (tile_total + 63) / 64)
 * @param tile_count: total number of tiles
 * @return index of tile to render, or -1 if all are done
 */
int	ren_dispatch(uint64_t *done_bitmap, int tile_total, int *tile_distributed);

/***
 * Take the number of the tile and return its x and y position in pixels
 * @param tile_index: index of the tile
 * @param res_x: resolution x of the image
 * @param x0: pointer to the x position of the tile
 * @param y0: pointer to the y position of the tile
 * @return void
 */
void	tile_index_to_pos(int tile_index, int res_x, int *x0, int *y0);

/***
 * Copy a buffer to an mlx image
 * @param img: destination image NON NULL
 * @param buf: source buffer NON NULL
 * @param src_pitch_px: pitch of the source buffer in pixels (width of the image
 * in pixels)
 * 
 * @note: the source buffer is in RGB format (3 int per pixel)
 * The destination image is in RGBA format (4 char per pixel)
 * The alpha channel is set to 255 (opaque)
 * 
 * @note: use memcpy for better performance
 */
void	buffer_to_mlx_image(t_image *img, const int *buf, int src_pitch_px);


/***
 * Copy a tile to the right position in the buffer
 * @param buf: source buffer
 * @param t_tile: tile to copy
 * @param src_pitch_px: pitch of the source buffer in pixels (width of the image
 * in pixels)
 * 
 * @note: use memcpy for better performance
 */
void	ren_tile_to_buffer(int *buf, const t_tile *tile, int src_pitch_px);

/*** 
 * initialize the rendering parameters for the rendering start
 * @param data: pointer to the t_data structure
 * @param cam: pointer to the t_cam structure
 */
void	render_begin(t_data *data, t_cam *cam);

/***
 * Create the threads for rendering
 * Each thread will render a tile
 * @param data: pointer to the t_data structure
 * @return SUCCESS or ERROR
 */
void	do_calculations(t_data *data);

/***
 * Worker thread function for rendering tiles
 * Each thread will repeatedly fetch and render tiles until all are done or
 * rendering is cancelled.
 * @param data: pointer to the t_data structure
 * @return NULL
 */
void	*worker_thread(void *data);

/***
 * Create the threads for rendering
 * Each thread will render a tile
 * @param data: pointer to the t_data structure
 * @return SUCCESS or ERROR
 */
int		create_threads(t_data *data);

#endif // RENDER_H