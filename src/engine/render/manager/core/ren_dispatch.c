/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ren_dispatch.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 19:35:23 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/17 23:03:48 by kearmand         ###   ########.fr       */
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

int	tile_permutation_step(int tile_count)
{
	uint32_t	seed;
	int			step;

	seed = 0xCAFEBABE;
	step = (rng_xorshift(&seed) % tile_count) | 1;
	while (gcd_int(step, tile_count) != 1)
		step += 2;
	return (step % tile_count);
}

int	manager_dispatch_tile(t_tileset *tileset)
{
	int	id;

	if (tileset->assigned == tileset->tiles_total)
		return (-1);
	id = *tileset->cursor;
	*tileset->cursor = (id + tileset->step) % tileset->tiles_total;
	tileset->assigned++;
	return (id);
}
