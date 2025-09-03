#include "render.h"
#include <stdlib.h>

/**
 * ren_free - free buffers inside render (framebuffer, bitmap, tiles)
 * @data: pointer to global data
 *
 * Does not reset fields to zero.
 */
void	ren_free(t_data *data)
{
	if (!data)
		return ;
	if (data->render.framebuffer)
	{
		free(data->render.framebuffer);
		data->render.framebuffer = NULL;
	}
	if (data->render.tiles_bitmap)
	{
		free(data->render.tiles_bitmap);
		data->render.tiles_bitmap = NULL;
	}
	ren_tiles_free(data);
}

/**
 * ren_destroy - full cleanup of render and reset fields
 * @data: pointer to global data
 *
 * Safe to call multiple times.
 */
void	ren_destroy(t_data *data)
{
	if (!data)
		return ;
	ren_free(data);
	data->render.width = 0;
	data->render.height = 0;
	data->render.tiles_total = 0;
	data->render.tiles_done = 0;
	data->render.tiles_distributed = 0;
	data->render.camera = NULL;
	data->render.samples_per_pixel = 1;
}
