/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 20:45:35 by norivier          #+#    #+#             */
/*   Updated: 2025/12/14 13:15:22 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bvh.h"
#include "rt_config.h"
#include "types.h"
#include "vector.h"
#include <float.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "libft.h"
#include "rt_math.h"

void	build_bvh(t_bvhnode *nodes, t_bvh_buf *buf, int primcount)
{
	int				sp;
	t_bvh_child		e;
	t_bvhnode		*node;
	int				mid;

	sp = 0;
	buf->stack[sp++] = (t_bvh_child){0, primcount, (buf->nodecount)++};
	while (sp > 0)
	{
		e = buf->stack[--sp];
		node = &nodes[e.node_idx];
		node->bounds = prim_bound_range(buf->pref,
			buf->pref_idx, e.start, e.count);
		node->is_leaf = 0;
		if (e.count <= LEAF_THRESHOLD)
		{
			node->is_leaf = 1;
			node->leaf.start = e.start;
			node->leaf.count = e.count;
			continue ;
		}
		mid = sah_split(e, buf);
		if (mid <= 0 || mid >= e.count)
			mid = e.count / 2;
		node->node.left = (buf->nodecount)++;
		node->node.right = (buf->nodecount)++;
		buf->stack[sp++] = (t_bvh_child){e.start, mid, node->node.left};
		buf->stack[sp++] = (t_bvh_child){e.start + mid, e.count - mid,
			node->node.right};
	}
}

FORCEINLINE
extern inline t_primitive	*reorder_prims(t_primitive *prims, int *idx,
	size_t count)
{
	t_primitive	*out;
	size_t		i;

	out = malloc(sizeof(out[0]) * count);
	if (out == NULL)
		return (NULL);
	i = 0;
	while (i < count)
	{
		out[i] = prims[idx[i]];
		i += 1;
	}
	return (out);
}

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
				if (prim_inter(r, &prims[prim_id], &local_hit) != 0)
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
