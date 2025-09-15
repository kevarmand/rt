/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: norivier <norivier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 20:45:35 by norivier          #+#    #+#             */
/*   Updated: 2025/09/14 21:59:34 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bvh.h"
#include "vector.h"
#include <math.h>
#include <stdlib.h>

t_aabb	triangle_bound(t_triangle *t)
{
	t_aabb	out;

	out.min = vec3f_min(vec3f_min(t->v0, t->v1), t->v2);
	out.max = vec3f_max(vec3f_max(t->v0, t->v1), t->v2);
	return (out);
}

t_vec3f	triangle_centroid(t_triangle *t)
{
	return ((t_vec3f){
		(t->v0.x + t->v1.x + t->v2.x) / 3.0f,
		(t->v0.y + t->v1.y + t->v2.y) / 3.0f,
		(t->v0.z + t->v1.z + t->v2.z) / 3.0f,
	});
}

t_aabb	sphere_bound(t_sphere *s)
{
	t_aabb	out;

	out.min = (t_vec3f){
		s->center.x - s->radius,
		s->center.y - s->radius,
		s->center.z - s->radius
	};
	out.max = (t_vec3f){
		s->center.x + s->radius,
		s->center.y + s->radius,
		s->center.z + s->radius
	};
	return (out);
}

t_vec3f	sphere_centroid(t_sphere *s)
{
	return (s->center);
}

t_aabb	cylinder_bound(t_cylinder *c)
{
	t_aabb	out;

	out.min = vec3f_min(c->p0, c->p1);
	out.max = vec3f_max(c->p0, c->p1);
	out.min = (t_vec3f){
		out.min.x - c->radius,
		out.min.y - c->radius,
		out.min.z - c->radius,
	};
	out.max = (t_vec3f){
		out.max.x + c->radius,
		out.max.y + c->radius,
		out.max.z + c->radius,
	};
	return (out);
}

t_vec3f	cylinder_centroid(t_cylinder *c)
{
	return ((t_vec3f){
		(c->p0.x + c->p1.x) / 2.0f,
		(c->p0.y + c->p1.y) / 2.0f,
		(c->p0.z + c->p1.z) / 2.0f
	});
}

t_aabb	torus_bound(t_torus *t)
{
	float	extent;
	t_aabb	out;

	extent = t->R + t->r;
	out.min = (t_vec3f){
		t->center.x - extent,
		t->center.y - extent,
		t->center.z - extent,
	};
	out.max = (t_vec3f){
		t->center.x + extent,
		t->center.y + extent,
		t->center.z + extent,
	};
	return (out);
}

t_vec3f	torus_centroid(t_torus *t)
{
	return (t->center);
}

t_aabb	quad_bound(t_quad *q)
{
	t_aabb	out;

	out.min = vec3f_min(vec3f_min(q->v0, q->v1), vec3f_min(q->v2, q->v3));
	out.max = vec3f_max(vec3f_max(q->v0, q->v1), vec3f_max(q->v2, q->v3));
	return (out);
}

t_vec3f	quad_centroid(t_quad *q)
{
	return ((t_vec3f){
		(q->v0.x + q->v1.x + q->v2.x + q->v3.x) / 4.0f,
		(q->v0.y + q->v1.y + q->v2.y + q->v3.y) / 4.0f,
		(q->v0.z + q->v1.z + q->v2.z + q->v3.z) / 4.0f,
	});
}

t_aabb	prim_bound(t_primitive *p)
{
	if (p->type == PRIM_TRIANGLE)
		return (triangle_bound(p->data));
	else if (p->type == PRIM_SPHERE)
		return (sphere_bound(p->data));
	else if (p->type == PRIM_CYLINDER)
		return (cylinder_bound(p->data));
	else if (p->type == PRIM_TORUS)
		return (torus_bound(p->data));
	else if (p->type == PRIM_QUAD)
		return (quad_bound(p->data));
	return ((t_aabb){{0, 0, 0}, {0, 0, 0}});
}

t_vec3f	prim_centroid(t_primitive *p)
{
	t_cylinder	*tmp;

	if (p->type == PRIM_TRIANGLE)
		return (triangle_centroid(p->data));
	else if (p->type == PRIM_SPHERE)
		return (sphere_centroid(p->data));
	else if (p->type == PRIM_CYLINDER)
		return (cylinder_centroid(p->data));
	else if (p->type == PRIM_TORUS)
		return (torus_centroid(p->data));
	else if (p->type == PRIM_QUAD)
		return (quad_centroid(p->data));
	return ((t_vec3f){0, 0, 0});
}

t_aabb	bound_merge(t_aabb a, t_aabb b)
{
	t_aabb	out;

	out.min = vec3f_min(a.min, b.min);
	out.max = vec3f_max(a.max, b.max);
	return (out);
}

float	bound_area(t_aabb b)
{
	t_vec3f	d;

	d.x = b.max.x - b.min.x;
	d.y = b.max.y - b.min.y;
	d.z = b.max.z - b.min.z;

	return (2.0f * (d.x * d.y + d.y * d.z + d.z * d.x));
}

t_aabb	prim_nbound(t_primpack pack)
{
	t_aabb	out;
	t_aabb	tmp;
	int		i;

	out = prim_bound(&pack.p[pack.indice[pack.start]]);
	i = 1;
	while (i < pack.count)
	{
		tmp = prim_bound(&pack.p[pack.indice[pack.start + i]]);
		out = bound_merge(out, tmp);
		i += 1;
	}
	return (out);
}

void	swap_int(int *a, int *b)
{
	int	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

float	prim_centroid_axis(t_primitive *p, int axis)
{
	t_vec3f	c;

	prim_centroid(p);
	if (axis == 0)
		return (c.x);
	if (axis == 1)
		return (c.y);
	return (c.z);
}

int	idx_partition(t_primpack pck, int low, int high, int axis)
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

int	sah_split(t_primpack pck, t_aabb nodebounds, int *axisout)
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
	bestcost = 1e30f;
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
		i = 0;
		while (i < pck.count)
		{
			tmp[i] = pck.indice[pck.start + i];
			i += 1;
		}
		quicksort_indices((t_primpack){pck.p, tmp, 0, 0},
			0, pck.count - 1, axis);
		left[0] = prim_bound(&pck.p[tmp[0]]);
		i = 0;
		while (i < pck.count)
		{
			left[i] = bound_merge(left[i - 1], prim_bound(&pck.p[tmp[i]]));
			i += 1;
		}
		right[pck.count - 1] = prim_bound(&pck.p[tmp[pck.count - 1]]);
		i = pck.count - 2;
		while (i >= 0)
		{
			right[i] = bound_merge(right[i + 1], prim_bound(&pck.p[tmp[i]]));
			i -= 1;
		}
		i = 1;
		while (i < pck.count)
		{
			cost = bound_area(left[i - 1]) * i +
				bound_area(right[i]) * (pck.count - i);
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

int	build_bvh(t_bvhnode *nodes, int *nodecount, t_primpack pck)
{
	int			nodeindex;
	t_bvhnode	*node;
	int			axis;
	int			mid;
	int			left;
	int			right;

	nodeindex = (*nodecount)++;
	node = &nodes[nodeindex];
	node->bounds = prim_nbound(pck);
	node->start = pck.start;
	node->count = pck.count;
	node->left = -1;
	node->right = -1;

	if (pck.count <= LEAF_THRESHOLD)
		return (nodeindex);
	axis = 0;
	mid = sah_split(pck, node->bounds, &axis);
	if (mid == MALLOC_ERR)
		return (MALLOC_ERR);
	if (mid <= 0 || mid >= pck.count)
		return (nodeindex);
	left = build_bvh(nodes, nodecount,
					(t_primpack){pck.p, pck.indice, pck.start, mid});
	if (left == MALLOC_ERR)
		return (MALLOC_ERR);
	right = build_bvh(nodes, nodecount, (t_primpack){pck.p, pck.indice,
					pck.start + mid, pck.count - mid});
	if (right == MALLOC_ERR)
		return (MALLOC_ERR);
	node->left = left;
	node->right = right;
	node->count = 0;
	return (nodeindex);
}

int	bound_intersect(t_ray r, t_aabb bound, float *near, float *far)
{
	t_aabb_inter	b;
	float			tmin;
	float			tmax;

	b.t1x = (bound.min.x = r.origin.x) * r.invdir.x;
	b.t2x = (bound.max.x = r.origin.x) * r.invdir.x;
	b.tminx = fminf(b.t1x, b.t2x);
	b.tmaxx = fmaxf(b.t1x, b.t2x);
	b.t1y = (bound.min.y = r.origin.y) * r.invdir.y;
	b.t2y = (bound.max.y = r.origin.y) * r.invdir.y;
	b.tminy = fminf(b.t1y, b.t2y);
	b.tmaxy = fmaxf(b.t1y, b.t2y);
	b.t1z = (bound.min.z = r.origin.z) * r.invdir.z;
	b.t2z = (bound.max.z = r.origin.z) * r.invdir.z;
	b.tminz = fminf(b.t1z, b.t2z);
	b.tmaxz = fmaxf(b.t1z, b.t2z);
	tmin = fmaxf(fmaxf(b.tminx, b.tminy), b.tminz);
	tmax = fminf(fminf(b.tmaxx, b.tmaxy), b.tmaxz);
	if (tmax < fmaxf(tmin, 0.0f))
		return (1);
	if (near != NULL)
		near[0] = tmin;
	if (far != NULL)
		far[0] = tmax;
	return (0);
}

// TEST BY THE CAT
#include <stdio.h>
#define prim_number 5
// ------------------------- example primitives -------------------------

// Recursively print the BVH tree with indentation
void print_bvh_tree(const t_bvhnode *nodes, int nodeIndex, int depth) {
    if (nodeIndex < 0) return;

    const t_bvhnode *n = &nodes[nodeIndex];

    // Indentation based on depth
    for (int i = 0; i < depth; i++) printf("  ");

    printf("Node %d: min(%.1f, %.1f, %.1f) max(%.1f, %.1f, %.1f), ",
           nodeIndex,
           n->bounds.min.x, n->bounds.min.y, n->bounds.min.z,
           n->bounds.max.x, n->bounds.max.y, n->bounds.max.z);

    printf("start=%d, count=%d, left=%d, right=%d\n",
           n->start, n->count, n->left, n->right);

    // Recurse on children if not a leaf
    if (n->left != -1)  print_bvh_tree(nodes, n->left, depth + 1);
    if (n->right != -1) print_bvh_tree(nodes, n->right, depth + 1);
}

// int main()
// {
//     // 1) allocate primitives
//     t_primitive prims[prim_number];
//
//     // triangle
//     t_triangle *tri = malloc(sizeof(t_triangle));
//     tri->v0 = (t_vec3f){0,0,0};
//     tri->v1 = (t_vec3f){1,0,0};
//     tri->v2 = (t_vec3f){0,1,0};
//     prims[0].type = PRIM_TRIANGLE;
//     prims[0].data = tri;
//
//     // sphere
//     t_sphere *s = malloc(sizeof(t_sphere));
//     s->center = (t_vec3f){2,2,2};
//     s->radius = 0.5f;
//     prims[1].type = PRIM_SPHERE;
//     prims[1].data = s;
//
//     // cylinder
//     t_cylinder *c = malloc(sizeof(t_cylinder));
//     c->p0 = (t_vec3f){-1,-1,-1};
//     c->p1 = (t_vec3f){-1,1,-1};
//     c->radius = 0.2f;
//     prims[2].type = PRIM_CYLINDER;
//     prims[2].data = c;
//
//     // quad
//     t_quad *q = malloc(sizeof(t_quad));
//     q->v0 = (t_vec3f){0,0,1};
//     q->v1 = (t_vec3f){1,0,1};
//     q->v2 = (t_vec3f){1,1,1};
//     q->v3 = (t_vec3f){0,1,1};
//     prims[3].type = PRIM_QUAD;
//     prims[3].data = q;
//
//     // torus
//     t_torus *t = malloc(sizeof(t_torus));
//     t->center = (t_vec3f){3,0,0};
//     t->R = 1.0f;
//     t->r = 0.3f;
//     prims[4].type = PRIM_TORUS;
//     prims[4].data = t;
//
//     // 2) allocate index array
//     int indices[prim_number] = {0,1,2,3,4};
//
//     // 3) allocate BVH node array (max nodes = 2*N-1)
//     t_bvhnode nodes[2 * prim_number - 1];
//     int nodecount = 0;
//
//     // 4) create primpack
//     t_primpack pack = {prims, indices, 0, prim_number};
//
//     // 5) build BVH
//     int root = build_bvh(nodes, &nodecount, pack);
//     if (root == -2)
//     {
//         printf("Malloc failed during BVH build!\n");
//         return 1;
//     }
//
//     // 6) print BVH
//     printf("BVH built, root = %d, total nodes = %d\n", root, nodecount);
// 	print_bvh_tree(nodes, root, 0);
//     // 7) free primitive data
//     free(tri);
//     free(s);
//     free(c);
//     free(q);
//     free(t);
//
//     return 0;
// }
