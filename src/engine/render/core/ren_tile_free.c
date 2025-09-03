#include "render.h"
#include <stdlib.h>
#include <stdatomic.h>

/**
 * tile_free - free one tile buffer and reset fields
 * @tile: pointer to the tile
 */
void	tile_free(t_tile *tile)
{
	if (!tile)
		return ;
	if (tile->buffer)
	{
		free(tile->buffer);
		tile->buffer = NULL;
	}
	tile->id = -1;
	tile->pos_x = 0;
	tile->pos_y = 0;
	tile->camera = NULL;
	tile->thread_id = (pthread_t)0;
	atomic_store(&tile->is_done, 1);
}

/**
 * ren_tiles_free - free all worker tiles
 * @data: pointer to global data
 */
void	ren_tiles_free(t_data *data)
{
	int	i;

	i = 0;
	while (i < NB_CORE)
	{
		tile_free(&(data->render.tiles[i]));
		i++;
	}
}
