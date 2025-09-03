#include <stdatomic.h>
#include "type.h"
#include <stdlib.h>
#include "render.h"

static inline int	round_up_mul(int n, int tile)
{
	return (((n + tile - 1) / tile) * tile);
}

static inline void	init_real_res(t_data *data)
{
	int	gw;
	int	gh;

	data->render.width = round_up_mul(data->para.res_width, TILE_SIZE);
	data->render.height = round_up_mul(data->para.res_height, TILE_SIZE);
	gw = data->render.width / TILE_SIZE;
	gh = data->render.height / TILE_SIZE;
	data->render.tiles_total = gw * gh;
}

int	ren_init(t_data *data)
{
	memset(&(data->render), 0, sizeof(t_render));
	init_real_res(data);
	data->render.framebuffer = malloc(sizeof(int) * data->render.width
			* data->render.height * RGB_CHANNELS);
	if (!data->render.framebuffer)
		return (ERROR);
	memset(data->render.framebuffer, 0, sizeof(int) * data->render.width
		* data->render.height * RGB_CHANNELS);
	data->render.tiles_bitmap = malloc(sizeof(uint64_t)
			* ((data->render.tiles_total + 63) / 64));
	if (!data->render.tiles_bitmap)
		return (ERROR);
	memset(data->render.tiles_bitmap, 0, sizeof(uint64_t)
		* ((data->render.tiles_total + 63) / 64));
	data->render.tiles_done = 0;
	atomic_store(&data->render.cancel_flag, 0);
	if (init_tiles(data) == ERROR)
		return (ERROR);
	return (SUCCESS);
}
