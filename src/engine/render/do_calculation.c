
#include "rt.h"
#include "render.h"


//do calculations for single core rendering

void	do_calculations(t_data *data)
{
	t_tile	*tile;
	int		tile_index;

	tile = &data->render.tiles[0];

	tile_index = ren_dispatch(data->render.tiles_bitmap,
		data->render.tiles_total, data->render.tiles_done);
	if (tile_index != -1)
	{
		tile->id = tile_index;
		tile->is_done = 0;
		render_tile(data, tile, data->render.camera);
		ren_tile_to_buffer(data->render.framebuffer, tile, data->render.width);
		tile->is_done = 1;
		data->render.tiles_done++;
	}
}