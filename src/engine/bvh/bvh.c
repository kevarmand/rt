/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 20:45:35 by norivier          #+#    #+#             */
/*   Updated: 2025/12/18 21:28:33 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bvh.h"
#include "rt_config.h"
#include "attributes.h"
#include "vector.h"
#include <float.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "libft.h"
#include "rt_math.h"

int	bvh_inter(t_ray r, t_bvhnode *nodes, t_primitive *prims, t_hit *out)
{
	uint32_t	stack[64];
	int			sp;
	int			hit_happened;

	hit_happened = 0;
	// out->t = FLT_MAX;
	sp = 0;
	stack[sp++] = 0;
	while (sp > 0)
	{
		uint32_t	idx = stack[--sp];
		t_bvhnode	*node = &nodes[idx];
		float		tnear;
		float		tfar;
		if (bound_intersect(r, node->bounds, &tnear, &tfar) == 0)
			continue ;
		if (tnear > out->t)
			continue ;
		if (node->is_leaf)
		{
			uint32_t	i;
			i = 0;
			while (i < node->leaf.count)
			{
				int	prim_id = node->leaf.start + i;
				t_hit	local_hit = *out;
				if (prim_inter(r, &prims[prim_id], &local_hit, tnear) != 0)
				{
					if (local_hit.t > TMIN_PRIM && local_hit.t < out->t)
					{
						*out = local_hit;
						out->primitive_id = prim_id;
						hit_happened = 1;
					}
				}
				i += 1;
			}
			continue ;
		}
		int	hitl; // so close
		int	hitr;
		float	tnearr;
		float	tfarr;

		hitl = bound_intersect(r, nodes[node->node.left].bounds, &tnear, &tfar);
		hitr = bound_intersect(r, nodes[node->node.right].bounds, &tnearr, &tfarr);
		if (hitl && tnear < out->t)
		{
			if (hitr && tnearr < out->t)
			{
				if (tnear < tnearr)
				{
					stack[sp++] = node->node.right;
					stack[sp++] = node->node.left;
				}
				else
				{
					stack[sp++] = node->node.left;
					stack[sp++] = node->node.right;
				}
			}
			else
				stack[sp++] = node->node.left;
		}
		else if (hitr && tnearr < out->t)
			stack[sp++] = node->node.right;
	}
	return (hit_happened);
}
