#include "render.h"
#include "type.h"

void	ren_tile_to_buffer(int *buf, const t_tile *tile, int src_pitch_px)
{
	int	x;
	int	y;
	int	row;

	row = 0;
	tile_index_to_pos(tile->id, src_pitch_px, &x, &y);
	while (row < TILE_SIZE)
	{
		memcpy(buf + RGB_CHANNELS * ((y + row) * src_pitch_px + x),
			tile->buffer + RGB_CHANNELS * row * TILE_SIZE,
			TILE_SIZE * sizeof(int) * RGB_CHANNELS);
		row++;
	}
}
