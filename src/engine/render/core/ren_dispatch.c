#include <stdint.h>
#include <limits.h>	

static int	tile_is_done(uint64_t *done_bitmap, int tile_id)
{
	int	word;
	int	bit;

	bit = tile_id % 64;
	word = tile_id / 64;
	return ((done_bitmap[word] >> bit) & 1);
}

static void	tile_mark_done(uint64_t *done_bitmap, int tile_id)
{
	int	word;
	int	bit;

	word = tile_id / 64;
	bit = tile_id % 64;
	done_bitmap[word] |= ((uint64_t)1 << bit);
}

static uint32_t	rng_next32(uint32_t *state)
{
	uint32_t	x;

	x = *state;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	*state = x;
	return (x);
}

int	ren_dispatch(uint64_t *done_bitmap, int tile_count,
					int *tile_distributed)
{
	static uint32_t	state = 0xCAFEBABE;
	int				tile_id;

	if (*tile_distributed >= tile_count)
		return (-1);
	tile_id = (int)(rng_next32(&state) % tile_count);
	while (1)
	{
		if (!tile_is_done(done_bitmap, tile_id))
		{
			tile_mark_done(done_bitmap, tile_id);
			(*tile_distributed)++;
			return (tile_id);
		}
		tile_id = (tile_id + 1) % tile_count;
	}
	return (-1);
}
