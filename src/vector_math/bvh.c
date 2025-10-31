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
#include <stdlib.h>
#include "libft.h"
#include "rt_math.h"

FORCEINLINE
extern inline float	prim_centroid_axis(t_primitive *p, int axis)
{
	if (axis == 0)
		return (p->centroid.x);
	if (axis == 1)
		return (p->centroid.y);
	return (p->centroid.z);
}

FORCEINLINE
extern inline float	prim_centroid_range(t_primpack pck, int axis)
{
	float	sum;
	int		i;

	sum = 0.0f;
	i = 0;
	while (i < pck.count)
	{
		sum += prim_centroid_axis(&pck.p[pck.indice[pck.start + i]], axis);
		i += 1;
	}
	return (sum / pck.count);
}

FORCEINLINE
extern inline int	idx_partition(t_primpack pck, int low, int high, int axis)
{
	float	pivot;
	int		i;
	int		j;

	pivot = prim_centroid_axis(&pck.p[pck.indice[high]], axis);
	i = low - 1;
	j = low;
	while (j < high)
	{
		if (prim_centroid_axis(&pck.p[pck.indice[j]], axis) <= pivot)
		{
			i += 1;
			swap_int(&pck.indice[i], &pck.indice[j]);
		}
		j += 1;
	}
	swap_int(&pck.indice[i + 1], &pck.indice[high]);
	return (i + 1);
}

FORCEINLINE
void	quicksort_indices(t_primpack pck, int low, int high, int axis)
{
	int	pi;

	if (low < high)
	{
		pi = idx_partition(pck, low, high, axis);
		quicksort_indices(pck, low, pi - 1, axis);
		quicksort_indices(pck, pi + 1, high, axis);
	}
}

int	sah_split(t_primpack pck, int *axisout)
{
	int		bestaxis;
	int		bestindex;
	float	bestcost;
	int		axis;
	int		*tmp;
	t_aabb	*left;
	t_aabb	*right;
	float	cost;
	int		i;
	int		j;

	bestaxis = -1;
	bestindex = -1;
	bestcost = FLT_MAX;
	if (pck.count < 2)
	{
		*axisout = 0;
		return (-1);
	}
	left = malloc(sizeof(t_aabb) * pck.count);
	if (left == NULL)
		return (MALLOC_ERR);
	right = malloc(sizeof(t_aabb) * pck.count);
	if (right == NULL)
		return (free(left), MALLOC_ERR);
	tmp = malloc(sizeof(int) * pck.count);
	if (tmp == NULL)
		return (free(right), free(left), MALLOC_ERR);
	axis = 0;
	while (axis < 3)
	{
		ft_memcpy(tmp, &pck.indice[pck.start], pck.count * sizeof(int));
		t_primpack	tmp_pack = {pck.p, tmp, 0, pck.count};
		quicksort_indices(tmp_pack, 0, pck.count - 1, axis);
		left[0][0] = pck.p[tmp[0]].bounds[0];
		left[0][1] = pck.p[tmp[0]].bounds[1];
		i = 1;
		while (i < pck.count)
		{
			bound_merge(left[i - 1], pck.p[tmp[i]].bounds, left[i]);
			i += 1;
		}
		right[pck.count - 1][0] = pck.p[tmp[pck.count - 1]].bounds[0];
		right[pck.count - 1][1] = pck.p[tmp[pck.count - 1]].bounds[1];
		i = pck.count - 2;
		while (i >= 0)
		{
			bound_merge(right[i + 1], pck.p[tmp[i]].bounds, right[i]);
			i -= 1;
		}
		i = 1;
		while (i < pck.count)
		{
			cost = bound_area(left[i - 1]) * i
				+ bound_area(right[i]) * (pck.count - i);
			if (cost < bestcost)
			{
				bestcost = cost;
				bestindex = i;
				bestaxis = axis;
				j = 0;
				while (j < pck.count)
				{
					pck.indice[pck.start + j] = tmp[j];
					j += 1;
				}
			}
			i += 1;
		}
		axis += 1;
	}
	free(left);
	free(right);
	free(tmp);
	*axisout = bestaxis;
	return (bestindex);
}

int	build_bvh(t_bvhnode *nodes, int *nodecount, t_primpack pck, int parent)
{
	int			nodeindex;
	t_bvhnode	*node;
	int			axis;
	int			mid;
	int			left;
	int			right;

	nodeindex = (*nodecount)++;
	node = &nodes[nodeindex];
	prim_nbound(pck, node->bounds);
	node->start = pck.start;
	node->count = pck.count;
	node->left = -1;
	node->right = -1;
	node->parent = parent;
	if (pck.count <= LEAF_THRESHOLD)
		return (nodeindex);
	axis = 0;
	mid = sah_split(pck, &axis);
	if (mid == MALLOC_ERR)
		return (MALLOC_ERR);
	if (mid <= 0 || mid >= pck.count)
		return (nodeindex);
	t_primpack	p_left = {pck.p, pck.indice, pck.start, mid};
	left = build_bvh(nodes, nodecount, p_left, nodeindex);
	if (left == MALLOC_ERR)
		return (MALLOC_ERR);
	t_primpack	p_right = {pck.p, pck.indice, pck.start + mid, pck.count - mid};
	right = build_bvh(nodes, nodecount, p_right, nodeindex);
	if (right == MALLOC_ERR)
		return (MALLOC_ERR);
	node->left = left;
	node->right = right;
	if (prim_centroid_range(p_left, axis) > prim_centroid_range(p_right, axis))
	{
		swap_int(&node->left, &node->right);
	}
	node->count = 0;
	return (nodeindex);
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

FORCEINLINE
extern inline t_vec3f	mat3x3_mulv(t_mat3x3 m, t_vec3f v)
{
	t_vec3f	res;

	res.x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z;
	res.y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z;
	res.z = m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z;
	return (res);
}

FORCEINLINE
extern inline t_mat3x3	mat3x3_transpose(t_mat3x3 m)
{
	t_mat3x3	t;

	t.m[0][0] = m.m[0][0];
	t.m[0][1] = m.m[1][0];
	t.m[0][2] = m.m[2][0];
	t.m[1][0] = m.m[0][1];
	t.m[1][1] = m.m[1][1];
	t.m[1][2] = m.m[2][1];
	t.m[2][0] = m.m[0][2];
	t.m[2][1] = m.m[1][2];
	t.m[2][2] = m.m[2][2];
	return (t);
}
// need matfunction to euler vector => matrice basis and inv_basis

// int	prim_inter(t_ray r, t_primitive *p, float *t)
// {
// 	if (p->type == PRIM_TRIANGLE)
// 		return (triangle_centroid(p->tr));
// 	else if (p->type == PRIM_SPHERE)
// 		return (sphere_centroid(p->sp));
// 	else if (p->type == PRIM_CYLINDER)
// 		return (cylinder_centroid(p->cy));
// 	else if (p->type == PRIM_TORUS)
// 		return (torus_centroid(p->to));
// 	else if (p->type == PRIM_QUAD)
// 		return (quad_centroid(p->qu));
// 	return (0);
// }
//
// t_hit	bvh2_parkour(t_ray r, t_bvhnode *nodes, t_primitive *prims, int root)
// {
// 	t_hit		out;
// 	t_bvhnode	*node;
// 	int			node_id;
// 	float		tnear;
// 	float		tfar;
//
// 	out = (t_hit){0, FLT_MAX, -1};
// 	node_id = root;
// 	while (node_id != -1)
// 	{
// 		node = &nodes[node_id];
//
// 		// cull if aabb missed or farther than best hit
// 		if (!bound_intersect(r, node->bounds, &tnear, &tfar) || tnear > out.t)
// 		{
// 			while (node_id != -1)
// 			{
// 				t_bvhnode	*parent;
// 				if (node->parent != -1)
// 					parent = &nodes[node->parent];
// 				else
// 					parent = NULL;
// 				if (parent && node_id == parent->left && parent->right != -1)
// 				{
// 					node_id = parent->right; // go to sibling
// 					break ;
// 				}
// 				else // backtrack
// 				{
// 					if (parent)
// 						node_id = parent->parent;
// 					else
// 						node_id = -1;
// 				}
// 			}
// 			continue ;
// 		}
// 		if (node->left == -1 && node->right == -1) // end leaf
// 		{
// 			int	i;
//
// 			i = 0;
// 			while (i < node->count)
// 			{
// 				int		prim_id;
// 				float	t;
// 				int		hit;
//
// 				prim_id = node->start + i;
// 				t = FLT_MAX;
// 				hit = prim_inter(r, &prims[prim_id], &t);
// 				if (hit && t < out.t && t > 0.0f)
// 				{
// 					out.hit = 1;
// 					out.t = t;
// 					out.prim_id = prim_id;
// 				}
// 				i += 1;
// 			}
// 			while (node_id != -1)
// 			{
// 				t_bvhnode	*parent;
// 				if (node->parent != -1)
// 					parent = &nodes[node->parent];
// 				else
// 					parent = NULL;
// 				if (parent && node_id == parent->left && parent->right != -1)
// 				{
// 					node_id = parent->right; // go to sibling
// 					break ;
// 				}
// 				else // backtrack
// 				{
// 					if (parent)
// 						node_id = parent->parent;
// 					else
// 						node_id = -1;
// 				}
// 			}
// 		}
// 		else
// 			node_id = node->left;
// 	}
// 	return (out);
// }
//
// TEST BY THE CAT
#include <stdio.h>
#define prim_number 2000
// ------------------------- example primitives -------------------------

// Recursively print the BVH tree with indentation
void print_bvh_tree(const t_bvhnode *nodes, int nodeIndex, int depth) {
    if (nodeIndex < 0) return;

    const t_bvhnode *n = &nodes[nodeIndex];

    // Indentation based on depth
    for (int i = 0; i < depth; i++) printf("  ");

    printf("Node %d: min(%.1f, %.1f, %.1f) max(%.1f, %.1f, %.1f), ",
           nodeIndex,
           n->bounds[0].x, n->bounds[0].y, n->bounds[0].z,
           n->bounds[1].x, n->bounds[1].y, n->bounds[1].z);

    printf("start=%d, count=%d, left=%d, right=%d\n",
           n->start, n->count, n->left, n->right);

    // Recurse on children if not a leaf
    if (n->left != -1)  print_bvh_tree(nodes, n->left, depth + 1);
    if (n->right != -1) print_bvh_tree(nodes, n->right, depth + 1);
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

void	fill_prim(t_primitive *p, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		p[i].type = PRIM_TRIANGLE;
		t_vec3f v0 = {(float)i, 0.0f, 0.0f};
		t_vec3f v1 = {(float)i + 0.5f, 1.0f, 0.0f};
		t_vec3f v2 = {(float)i + 1.0f, 0.0f, 0.0f};
		p[i].tr = make_triangle(v0, v1, v2);
		prim_bound(&p[i]);
		p[i].centroid = prim_centroid(&p[i]);
		i += 1;
	}
}

int main()
{
    // 1) allocate primitives
    t_primitive prims[prim_number];
    // triangle
	fill_prim(prims, prim_number);
    // 2) allocate index array
    int indices[prim_number];
	for (int i = 0; i < prim_number; i += 1)
	{
		indices[i] = i;
	}
    // 3) allocate BVH node array (max nodes = 2*N-1)
    t_bvhnode nodes[2 * prim_number - 1];
    int nodecount = 0;

    // 4) create primpack
    t_primpack pack = {prims, indices, 0, prim_number};

    // 4.5) precompute aabb for each primitive
    prim_bound_init(pack);

    // 5) build BVH
    int root = build_bvh(nodes, &nodecount, pack, -1);
    if (root == -2)
    {
        printf("Malloc failed during BVH build!\n");
        return 1;
    }
    // 6) print BVH
    printf("BVH built, root = %d, total nodes = %d\n", root, nodecount);
	print_bvh_tree(nodes, root, 0);
    return 0;
}
