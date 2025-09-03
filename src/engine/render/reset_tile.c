

#include "rt.h"
#include "render.h"

void	reset_tile(uint64_t *done_bits, int tile_total, int *tile_done)
{
	int	words = (tile_total + 63) / 64;
	int	i;

	i = 0;
	while (i < words)
	{
		done_bits[i] = 0;
		i++;
	}
	*tile_done = 0;
}
