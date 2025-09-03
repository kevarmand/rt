#include "render.h"
#include "type.h"
#include <stdlib.h>
#include <stdatomic.h>

/***
 * Malloc and initialize the tiles
 * For each tile allocate the buffer
 * Initialize the tile_current to -1
 * Initialize the tile_buffer to 0
 * Initialize the done_bits to 0
 * @param data: pointer to the t_data structure
 * @return SUCCESS or ERROR
 */
static int	tiles_init(t_tile *tile)
{
	size_t	elems;
	size_t	bytes;

	elems = (size_t)TILE_SIZE * (size_t)TILE_SIZE * (size_t)RGB_CHANNELS;
	bytes = elems * sizeof(*tile->buffer);
	tile->buffer = malloc(bytes);
	if (!tile->buffer)
		return (ERROR);
	memset(tile->buffer, 0, bytes);
	tile->id = -1;
	tile->pos_x = 0;
	tile->pos_y = 0;
	atomic_store(&tile->is_done, 1);
	return (SUCCESS);
}

int	init_tiles(t_data *data)
{
	int	i;

	i = 0;
	if (NB_CORE == 1)
		return (tiles_init(&(data->render.tiles[0])));
	while (i < NB_CORE - 1)
	{
		if (tiles_init(&(data->render.tiles[i])) == ERROR)
			return (ERROR);
		i++;
	}
	return (SUCCESS);
}
