#include "render.h"
#include "mlx.h"


void	tile_index_to_pos(int tile_index, int res_x, int *x0, int *y0)
{
	int gw;

	gw = res_x / TILE_SIZE;
	*x0 = (tile_index % gw) * TILE_SIZE;
	*y0 = (tile_index / gw) * TILE_SIZE;
}