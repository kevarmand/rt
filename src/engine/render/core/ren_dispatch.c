/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ren_dispatch.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 19:35:23 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/28 16:34:18 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <limits.h>	
#include "render.h"


static int	gcd_int(int left, int right)
{
	int	tmp;

	while (right != 0)
	{
		tmp = left % right;
		left = right;
		right = tmp;
	}
	return (left);
}

static uint32_t	rng_xorshift(uint32_t *s)
{
	uint32_t	x;

	x = *s;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	*s = x;
	return (x);
}

static int	dispatch(int tile_count)
{
	static int			k = 0;
	static int			step = -1;
	static uint32_t		rng = 0xCAFEBABE;

	if (tile_count <= 0)
	{
		k = 0;
		step = -1;
		return (0);
	}
	if (step < 0)
	{
		step = (int)(rng_xorshift(&rng) % tile_count);
		if ((step & 1) == 0)
			step++;
		while (gcd_int(step, tile_count) != 1)
			step += 2;
	}
	if (k >= tile_count)
		k = 0;
	return ((k++ * step) % tile_count);
}

int manager_dispatch_tile(t_tileset *tileset)
{
	if (tileset->tiles_done + tileset->tiles_active + tileset->tiles_ready >= tileset->tiles_total)
		return (-1);
	else
		return (dispatch(tileset->tiles_total));
}
