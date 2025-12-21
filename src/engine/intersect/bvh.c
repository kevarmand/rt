/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 20:45:35 by norivier          #+#    #+#             */
/*   Updated: 2025/12/21 03:11:00 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bvh.h"
#include <stdio.h>

__attribute__((always_inline))
static inline void	bvh_inter1(t_bvh_inter_ctx *c, t_ray r, t_hit *out)
{
	c->valid_l = (bound_intersect(r, *c->left_bounds, &c->tnear, NULL)
			&& c->tnear < out->t);
	c->valid_r = (bound_intersect(r, *c->right_bounds, &c->tnearr, NULL)
			&& c->tnearr < out->t);
	if (c->valid_l && c->valid_r)
	{
		if (c->tnear < c->tnearr)
		{
			c->stack[c->sp++] = c->node->node.right;
			c->stack[c->sp++] = c->node->node.left;
		}
		else
		{
			c->stack[c->sp++] = c->node->node.left;
			c->stack[c->sp++] = c->node->node.right;
		}
	}
	else if (c->valid_l)
		c->stack[c->sp++] = c->node->node.left;
	else if (c->valid_r)
		c->stack[c->sp++] = c->node->node.right;
}

__attribute__((always_inline))
static inline void	bvh_inter0(t_bvh_inter_ctx *c, t_ray r,
		t_primitive *prims, t_hit *out)
{
	uint32_t	i;
	int			prim_id;
	t_hit		local_hit;

	i = 0;
	while (i < c->node->leaf.count)
	{
		prim_id = c->node->leaf.start + i;
		local_hit = *out;
		if (prim_inter(r, &prims[prim_id], &local_hit, c->tnear) != 0)
		{
			if (local_hit.t > TMIN_PRIM && local_hit.t < out->t)
			{
				*out = local_hit;
				out->primitive_id = prim_id;
				c->hit_happened = 1;
			}
		}
		i += 1;
	}
}

int	bvh_inter(t_ray r, t_bvhnode *nodes, t_primitive *prims, t_hit *out)
{
	t_bvh_inter_ctx	c;

	c.hit_happened = 0;
	c.sp = 0;
	c.stack[c.sp++] = 0;
	while (c.sp > 0)
	{
		c.node = &nodes[c.stack[--c.sp]];
		if (bound_intersect(r, c.node->bounds, &c.tnear, NULL) == 0)
			continue ;
		if (c.tnear > out->t)
			continue ;
		if (c.node->is_leaf)
		{
			bvh_inter0(&c, r, prims, out);
			continue ;
		}
		c.left_bounds = &nodes[c.node->node.left].bounds;
		c.right_bounds = &nodes[c.node->node.right].bounds;
		bvh_inter1(&c, r, out);
	}
	return (c.hit_happened);
}

__attribute__((always_inline))
static inline int	bvh_shadow0(t_bvh_inter_ctx *c, t_ray r,
		t_primitive *prims, t_hit *out)
{
	uint32_t	i;
	int			prim_id;
	t_hit		local_hit;

	i = 0;
	while (i < c->node->leaf.count)
	{
		prim_id = c->node->leaf.start + i;
		local_hit = *out;
		if (prim_inter(r, &prims[prim_id], &local_hit, c->tnear) != 0)
		{
			if (local_hit.t > 0.0f && local_hit.t < out->t)
				return (1);
		}
		i += 1;
	}
	return (0);
}

int	bvh_shadow(t_ray r, t_bvhnode *nodes, t_primitive *prims, t_hit *out)
{
	t_bvh_inter_ctx	c;

	c.sp = 0;
	c.stack[c.sp++] = 0;
	while (c.sp > 0)
	{
		c.node = &nodes[c.stack[--c.sp]];
		if (bound_intersect(r, c.node->bounds, &c.tnear, NULL) == 0)
			continue ;
		if (c.tnear > out->t)
			continue ;
		if (c.node->is_leaf)
		{
			if (bvh_shadow0(&c, r, prims, out))
				return (1);
			continue ;
		}
		c.left_bounds = &nodes[c.node->node.left].bounds;
		c.right_bounds = &nodes[c.node->node.right].bounds;
		bvh_inter1(&c, r, out);
	}
	return (0);
}
