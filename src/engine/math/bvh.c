/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 20:45:35 by norivier          #+#    #+#             */
/*   Updated: 2025/10/31 05:40:44 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bvh.h"
#include "types.h"
#include "vector.h"
#include <float.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "libft.h"
#include "rt_math.h"

FORCEINLINE
extern inline float	prim_centroid_axis(const t_primref *p, int axis)
{
	if (axis == 0)
		return (p->centroid.x);
	if (axis == 1)
		return (p->centroid.y);
	return (p->centroid.z);
}

FORCEINLINE
t_aabb	prim_bound_range(t_primref *pref, int *indice, int start, int count)
{
	t_aabb	out;
	int		i;

	out = pref[indice[start]].bounds;
	i = 1;
	while (i < count)
	{
		out = bound_merge(out, pref[indice[start + i]].bounds);
		i += 1;
	}
	return (out);
}

FORCEINLINE
void	zero_bins(t_bin *bins, size_t siz)
{
	size_t	i;

	i = 0;
	while (i < siz)
	{
		bins[i].count = 0;
		bins[i].bounds.b[0] = (t_vec3f){FLT_MAX, FLT_MAX, FLT_MAX, 0.0f};
		bins[i].bounds.b[1] = (t_vec3f){-FLT_MAX, -FLT_MAX, -FLT_MAX, 0.0f};
		i += 1;
	}
}

void	instance_cbounds(t_bvh_buf *b, t_bvh_child e, t_vec3f *cbounds)
{
	t_vec3f	tmp_c;
	int		i;

	cbounds[0] = b->pref[b->pref_idx[e.start]].centroid;
	cbounds[1] = cbounds[0];
	i = 1;
	while (i < e.count)
	{
		tmp_c = b->pref[b->pref_idx[e.start + i]].centroid;
		cbounds[0] = vec3f_min(cbounds[0], tmp_c);
		cbounds[1] = vec3f_max(cbounds[1], tmp_c);
		i += 1;
	}
}

FORCEINLINE
extern inline int	fill_bins(t_bvh_buf *b,t_bvh_child e, t_vec3f *cbounds,
	int axis)
{
	int		i;
	float	tmp_c;
	int		bidx;
	t_bin	*bin;

	if (cbounds[1][axis] - cbounds[0][axis] <= EPSILON)
		return (1);
	i = 0;
	while (i < e.count)
	{
		tmp_c = prim_centroid_axis(&b->pref[b->pref_idx[e.start + i]], axis);
		bidx = (int)(((tmp_c - cbounds[0][axis])
			/ (cbounds[1][axis] - cbounds[0][axis])) * (float)BIN_COUNT);
		if (bidx == BIN_COUNT)
			bidx = BIN_COUNT - 1;
		bin = &b->bins[bidx];
		bin->bounds = bound_merge(bin->bounds,
			b->pref[b->pref_idx[e.start + i]].bounds);
		bin->count += 1;
		i += 1;
	}
	return (0);
}

FORCEINLINE
extern inline void	build_prefix_suffix(t_bvh_buf *b)
{
	int	i;

	b->prefix[0] = b->bins[0].bounds;
	i = 1;
	while (i < BIN_COUNT)
	{
		if (b->bins[i].count)
			b->prefix[i] = bound_merge(b->prefix[i - 1], b->bins[i].bounds);
		else
			b->prefix[i] = b->prefix[i - 1];
		i += 1;
	}
	b->suffix[BIN_COUNT - 1] = b->bins[BIN_COUNT - 1].bounds;
	i = BIN_COUNT - 2;
	while (i >= 0)
	{
		if (b->bins[i].count)
			b->suffix[i] = bound_merge(b->suffix[i + 1], b->bins[i].bounds);
		else
			b->suffix[i] = b->suffix[i + 1];
		i -= 1;
	}
}

FORCEINLINE
extern inline void	prefix_suffix_count(t_bvh_buf *b)
{
	int	i;
	int	sum;

	i = 0;
	sum = 0;
	while (i < BIN_COUNT)
	{
		sum += b->bins[i].count;
		b->left_counts[i] = sum;
		i += 1;
	}
}

FORCEINLINE
extern inline void	find_bestcost(t_bvh_buf *b, t_bvh_child e, int axis)
{
	int		i;
	int		counts[2];
	float	cost;

	i = -1;
	while (i < BIN_COUNT - 1)
	{
		i += 1;
		counts[0] = b->left_counts[i];
		counts[1] = e.count - counts[0];
		if (counts[0] == 0 || counts[1] == 0)
			continue ;
		cost = bound_area(b->prefix[i]) * (float)counts[0]
			+ bound_area(b->suffix[i + 1]) * (float)counts[1];
		if (cost < b->bestcost)
		{
			b->bestcost = cost;
			b->bestaxis = axis;
			b->bin_split_idx = i;
		}
	}
}

FORCEINLINE
extern inline int	partition_inplace(t_bvh_buf *b, t_bvh_child e,
	t_vec3f *cbounds)
{
	int		low;
	int		high;
	float	cent;
	int		bidx;

	low = e.start;
	high = e.start + e.count - 1;
	while (low <= high)
	{
		cent = prim_centroid_axis(&b->pref[b->pref_idx[low]], b->bestaxis);
		bidx = (int)(((cent - cbounds[0][b->bestaxis])
			/ (cbounds[1][b->bestaxis]
				- cbounds[0][b->bestaxis])) * BIN_COUNT);
		if (bidx >= BIN_COUNT)
			bidx = BIN_COUNT - 1;
		if (bidx > b->bin_split_idx)
		{
			swap_int(&b->pref_idx[low], &b->pref_idx[high]);
			high -= 1;
		}
		else
			low += 1;
	}
	return (low - e.start);
}

int	sah_split(t_bvh_child e, t_bvh_buf *b)
{
	t_vec3f	cbounds[2];
	int		split_idx;
	int		axis;

	if (e.count < 2)
		return (-1);
	b->bestcost = FLT_MAX;
	b->bestaxis = -1;
	b->bin_split_idx = -1;
	instance_cbounds(b, e, cbounds);
	axis = -1;
	while (axis < 3)
	{
		axis += 1;
		zero_bins(b->bins, BIN_COUNT);
		if (fill_bins(b, e, cbounds, axis) != 0)
			continue ;
		build_prefix_suffix(b);
		prefix_suffix_count(b);
		find_bestcost(b, e, axis);
	}
	if (b->bestaxis < 0)
		return (-1);
	split_idx = partition_inplace(b, e, cbounds);
	return (split_idx);
}
#include <stdio.h>

void	shuffle(int *array, size_t n)
{
	if (n > 1)
	{
		size_t	i;
		for (i = 0; i < n - 1; i++)
		{
			size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
			swap_int(&array[i], &array[j]);
		}
	}
}

int	build_bvh(t_bvhnode *nodes, t_bvh_buf *buf, int primcount)
{
	t_bvh_child		*stack;
	int				sp;
	int				i;

	// init process to move in another function
	buf->pref_idx = malloc(sizeof(int) * primcount);
	if (buf->pref_idx == NULL)
		return (MALLOC_ERR);
	stack = malloc(sizeof(stack[0]) * (primcount * 2 + 1)); // stack of primcount should be enough
	if (stack == NULL)
		return (free(buf->pref_idx), MALLOC_ERR);
	sp = 0;
	buf->nodecount = 0;
	i = 0;
	while (i < primcount)
	{
		buf->pref_idx[i] = i;
		i += 1;
	}
	shuffle(buf->pref_idx, primcount); // this is for testing only
	stack[sp++] = (t_bvh_child){0, primcount, (buf->nodecount)++};
	while (sp > 0)
	{
		t_bvh_child	e = stack[--sp];
		t_bvhnode	*node = &nodes[e.node_idx];

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
		int	mid = sah_split(e, buf);
		if (mid <= 0 || mid >= e.count)
			mid = e.count / 2;
		node->node.left = (buf->nodecount)++;
		node->node.right = (buf->nodecount)++;
		stack[sp++] = (t_bvh_child){e.start, mid, node->node.left};
		stack[sp++] = (t_bvh_child){e.start + mid, e.count - mid,
			node->node.right};
	}
	free(stack);
	return (buf->nodecount);
}

FORCEINLINE
t_ray	build_ray(t_vec3f origin, t_vec3f direction)
{
	t_ray	out;

	out.origin = origin;
	out.dir = vec3f_normalize(direction);
	out.invdir.x = 1.0f / direction.x;
	out.invdir.y = 1.0f / direction.y;
	out.invdir.z = 1.0f / direction.z;
	out.orig_div.x = origin.x * out.invdir.x;
	out.orig_div.y = origin.y * out.invdir.y;
	out.orig_div.z = origin.z * out.invdir.z;
	out.sign[0] = (out.invdir.x < 0);
	out.sign[1] = (out.invdir.y < 0);
	out.sign[2] = (out.invdir.z < 0);
	return (out);
}

// FORCEINLINE
// extern inline t_mat3x4f	build_affine(const t_transform t)
// {
// 	return ((t_mat3x4f){
// 		{t.rot[0][0] * t.scale.x, t.rot[0][1] * t.scale.y, t.rot[0][2] * t.scale.z, t.trans.x},
// 		{t.rot[1][0] * t.scale.x, t.rot[1][1] * t.scale.y, t.rot[1][2] * t.scale.z, t.trans.x},
// 		{t.rot[2][0] * t.scale.x, t.rot[2][1] * t.scale.y, t.rot[2][2] * t.scale.z, t.trans.x},
// 	});
// }

// t_hit	bvh2_parkour(t_ray r, t_bvhnode *nodes, t_bvh_buf *b)
// {
// 	t_hit		out;
// 	uint32_t	stack[64];
// 	int			sp;
//
// 	out.t = FLT_MAX;
// 	stack[sp++] = 0;
// 	while (sp > 0)
// 	{
// 		uint32_t	idx = stack[--sp];
// 		t_bvhnode	*node = &nodes[idx];
// 		float		tnear;
// 		float		tfar;
// 		if (bound_intersect(r, node->bounds, &tnear, &tfar) == 0)
// 			continue ;
// 		if (tnear > out.t)
// 			continue ;
// 		if (node->is_leaf)
// 		{
// 			uint32_t	i;
//
// 			i = 0;
// 			while (i < node->leaf.count)
// 			{
// 				int	prim_id = b->pref_idx[node->leaf.start + i];
// 				t_hit	local_hit = out;
// 				if (prim_inter(b->pref[prim_id], r, &local_hit) != 0)
// 				{
// 					if (local_hit.t > EPSILON && local_hit.t < out.t)
// 					{
// 						out = local_hit;
// 						out.prim_id = prim_id;
// 					}
// 				}
// 				i += 1;
// 			}
// 			continue ;
// 		}
// 		int	hitl; // so close
// 		int	hitr;
// 		float	tnearr;
// 		float	tfarr;
//
// 		hitl = bound_intersect(r, nodes[node->node.left].bounds, &tnear, &tfar);
// 		hitr = bound_intersect(r, nodes[node->node.right].bounds, &tnearr, &tfarr);
// 		if (hitl && tnear < out.t)
// 		{
// 			if (hitr && tnearr < out.t)
// 			{
// 				if (tnear < tnearr)
// 				{
// 					stack[sp++] = node->node.right;
// 					stack[sp++] = node->node.left;
// 				}
// 				else
// 				{
// 					stack[sp++] = node->node.left;
// 					stack[sp++] = node->node.right;
// 				}
// 			}
// 			else
// 				stack[sp++] = node->node.left;
// 		}
// 		else if (hitr && tnearr < out.t)
// 			stack[sp++] = node->node.right;
// 	}
// 	return (out);
// }
// TEST BY THE CAT
#include <stdio.h>
// ------------------------ example primitives -------------------------

// Recursively print the BVH tree with indentation
void print_bvh_tree(const t_bvhnode *nodes, int nodeIndex, int depth) {
    const t_bvhnode *n = &nodes[nodeIndex];

    // Indentation based on depth

	if (n->is_leaf == 1)
	{
		t_vec3f	min = n->bounds.b[0];
		t_vec3f	max = n->bounds.b[1];
		printf("min: [%0.2f,%0.2f,%0.2f], max: [%0.2f,%0.2f,%0.2f], leaf:%d, count:%d\n", min.x, min.y, min.z, max.x, max.y, max.z,n->is_leaf, n->leaf.count);
	}
	   // Recurse on children if not a leaf
	if (n->is_leaf == 0)
	{
		print_bvh_tree(nodes, n->node.left, depth + 1);
		print_bvh_tree(nodes, n->node.right, depth + 1);
	}
}

t_triangle	make_triangle(t_vec3f v0, t_vec3f v1, t_vec3f v2)
{
	t_triangle	t;

	t.v0 = v0;
	t.edge1 = vec3f_sub(v1, v0);
	t.edge2 = vec3f_sub(v2, v0);
	t.normal = vec3f_normalize(vec3f_cross(t.edge1, t.edge2)); // make sure we both are okay on right hand rule here
	return (t);
}

float	randf(float max)
{
	return (((float)rand()/(float)RAND_MAX) * max);
}

void	fill_prim(t_primitive *p, int len)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < len)
	{
		j += 1;
		p[i].type = PRIM_TRIANGLE;
		t_vec3f v0 = {i + randf(i + j), i + randf(i + j), i + randf(i + j)};
		t_vec3f v1 = {i + randf(i + j), i + randf(i + j), i + randf(i + j)};
		t_vec3f v2 = {i + randf(i + j), i + randf(i + j), i + randf(i + j)};
		p[i].tr = make_triangle(v0, v1, v2);
		prim_bound(&p[i]);
		i += 1;
	}
}

int main(int argc, char *argv[])
{
    // 1) allocate primitives
	int prim_number = 0;
	if (argc > 1)
		prim_number = atoi(argv[1]);
	if (prim_number <= 0)
		prim_number = 16;
	t_bvhnode *nodes = malloc(sizeof(nodes[0]) * (prim_number));
	if (nodes == NULL)
		return (42);
	t_primref	*prefs = malloc(sizeof(prefs[0]) * prim_number);
	t_primitive	*prims = malloc(sizeof(prims[0]) * prim_number);
	t_bvh_buf	buf;
	int	nodecount = 0;
    // triangle
	fill_prim(prims, prim_number);
    // 4.5) precompute aabb for each primitive
    prim_bound_init(prefs, prims, prim_number);

    // 5) build BVH
	printf("primitive count: %d\n", prim_number);
	buf.pref = prefs;
	nodecount = build_bvh(nodes, &buf, prim_number);
	if (nodecount > 0)
	{
		printf("BVH built, total nodes = %d\n", nodecount);
		print_bvh_tree(nodes, 0, 0);
	}
	free(nodes);
	free(prefs);
	free(buf.pref_idx);
	free(prims);
    return 0;
}
