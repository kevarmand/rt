/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_build.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: norivier <norivier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 15:55:16 by norivier          #+#    #+#             */
/*   Updated: 2025/12/20 21:34:06 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bvh.h"
#include "scene.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

__attribute__((always_inline))
static inline void	build_bvh0(t_bvhnode *node, t_bvh_buf *buf, t_bvh_child *e)
{
	int				mid;

	mid = sah_split(*e, buf);
	if (mid <= 0 || mid >= e->count)
		mid = e->count / 2;
	node->node.left = (buf->nodecount)++;
	node->node.right = (buf->nodecount)++;
	buf->stack[buf->sp++] = (t_bvh_child){e->start, mid, node->node.left};
	buf->stack[buf->sp++] = (t_bvh_child){e->start + mid, e->count - mid,
		node->node.right};
}

void	build_bvh(t_bvhnode *nodes, t_bvh_buf *buf, int primcount)
{
	t_bvh_child		e;
	t_bvhnode		*node;

	buf->sp = 0;
	buf->stack[buf->sp++] = (t_bvh_child){0, primcount, (buf->nodecount)++};
	while (buf->sp > 0)
	{
		e = buf->stack[--(buf->sp)];
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
		build_bvh0(node, buf, &e);
	}
}

__attribute__((always_inline))
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
