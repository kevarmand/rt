/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 20:45:35 by norivier          #+#    #+#             */
/*   Updated: 2025/10/02 12:02:14 by norivier         ###   ########.fr       */
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

#define RCP_2 0x1p-1f
#define RCP_3 0x1.555556p-2f
#define RCP_4 0x1p-2f
#define RCP_8 0x1p-3f
#define RCP_9 0x1.c71c72p-4f
#define RCP_16 0x1p-4f
#define RCP_256 0x1p-8f
#define THREE_OVER_8 0x1.8p-2f
#define THREE_OVER_256 0x1.8p-2f

FORCEINLINE
extern inline void	triangle_bound(t_triangle t, t_aabb out)
{
	t_vec3f	v1;
	t_vec3f	v2;

	v1 = vec3f_add(t.v0, t.edge1);
	v2 = vec3f_add(t.v0, t.edge2);
	out[0] = vec3f_min(vec3f_min(t.v0, v1), v2);
	out[1] = vec3f_max(vec3f_max(t.v0, v1), v2);
}

FORCEINLINE
extern inline t_vec3f	triangle_centroid(t_triangle t)
{
	t_vec3f	out;
	t_vec3f	v1;
	t_vec3f	v2;

	v1 = vec3f_add(t.v0, t.edge1);
	v2 = vec3f_add(t.v0, t.edge2);
	out.x = (t.v0.x + v1.x + v2.x) / 3.0f;
	out.y = (t.v0.y + v1.y + v2.y) / 3.0f;
	out.z = (t.v0.z + v1.z + v2.z) / 3.0f;
	return (out);
}

FORCEINLINE
extern inline void	sphere_bound(t_sphere s, t_aabb out)
{
	out[0].x = s.center.x - s.radius;
	out[0].y = s.center.y - s.radius;
	out[0].z = s.center.z - s.radius;
	out[1].x = s.center.x + s.radius;
	out[1].y = s.center.y + s.radius;
	out[1].z = s.center.z + s.radius;
}

FORCEINLINE
extern inline t_vec3f	sphere_centroid(t_sphere s)
{
	return (s.center);
}

FORCEINLINE
extern inline void	cylinder_bound(t_cylinder c, t_aabb out)
{
	out[0] = vec3f_min(c.p0, c.p1);
	out[1] = vec3f_max(c.p0, c.p1);
	out[0].x -= c.radius;
	out[0].y -= c.radius;
	out[0].z -= c.radius;
	out[1].x += c.radius;
	out[1].y += c.radius;
	out[1].z += c.radius;
}

FORCEINLINE
extern inline t_vec3f	cylinder_centroid(t_cylinder c)
{
	t_vec3f	out;

	out.x = (c.p0.x + c.p1.x) / 2.0f;
	out.y = (c.p0.y + c.p1.y) / 2.0f;
	out.z = (c.p0.z + c.p1.z) / 2.0f;
	return (out);
}

FORCEINLINE
extern inline void	torus_bound(t_torus t, t_aabb out)
{
	float	extent;

	extent = t.R + t.r;
	out[0].x = t.center.x - extent;
	out[0].y = t.center.y - extent;
	out[0].z = t.center.z - extent;
	out[1].x = t.center.x + extent;
	out[1].y = t.center.y + extent;
	out[1].z = t.center.z + extent;
}

FORCEINLINE
extern inline t_vec3f	torus_centroid(t_torus t)
{
	return (t.center);
}

FORCEINLINE
extern inline void	prim_bound(t_primitive *p)
{
	t_aabb	out;

	if (p->type == PRIM_TRIANGLE)
		triangle_bound(p->tr, out);
	else if (p->type == PRIM_SPHERE)
		sphere_bound(p->sp, out);
	else if (p->type == PRIM_CYLINDER)
		cylinder_bound(p->cy, out);
	else if (p->type == PRIM_TORUS)
		torus_bound(p->to, out);
	else
	{
		out[0] = (t_vec3f){0, 0, 0};
		out[1] = (t_vec3f){0, 0, 0};
	}
	p->bounds[0] = out[0];
	p->bounds[1] = out[1];
}

FORCEINLINE
extern inline t_vec3f	prim_centroid(t_primitive *p)
{
	if (p->type == PRIM_TRIANGLE)
		return (triangle_centroid(p->tr));
	else if (p->type == PRIM_SPHERE)
		return (sphere_centroid(p->sp));
	else if (p->type == PRIM_CYLINDER)
		return (cylinder_centroid(p->cy));
	else if (p->type == PRIM_TORUS)
		return (torus_centroid(p->to));
	return ((t_vec3f){0, 0, 0});
}

FORCEINLINE
extern inline void	bound_merge(t_aabb a, t_aabb b, t_aabb out)
{
	out[0] = vec3f_min(a[0], b[0]);
	out[1] = vec3f_max(a[1], b[1]);
}

FORCEINLINE
extern inline float	bound_area(t_aabb b)
{
	t_vec3f	d;

	d.x = b[1].x - b[0].x;
	d.y = b[1].y - b[0].y;
	d.z = b[1].z - b[0].z;
	return (2.0f * (d.x * d.y + d.y * d.z + d.z * d.x));
}

FORCEINLINE
extern inline void	prim_bound_init(t_primpack pack)
{
	t_primitive	*p;
	int			i;

	i = 0;
	while (i < pack.count)
	{
		p = &pack.p[pack.indice[pack.start + i]];
		prim_bound(p);
		i += 1;
	}
}

FORCEINLINE
extern inline void	prim_nbound(t_primpack pack, t_aabb out)
{
	t_aabb	tmp;
	int		i;

	out[0] = pack.p[pack.indice[pack.start]].bounds[0];
	out[1] = pack.p[pack.indice[pack.start]].bounds[1];
	i = 1;
	while (i < pack.count)
	{
		tmp[0] = pack.p[pack.indice[pack.start + i]].bounds[0];
		tmp[1] = pack.p[pack.indice[pack.start + i]].bounds[1];
		bound_merge(out, tmp, out);
		i += 1;
	}
}

FORCEINLINE
extern inline void	swap_int(int *a, int *b)
{
	int	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

FORCEINLINE
extern inline void	swap_float(float *a, float *b)
{
	float	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

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
extern inline int	bound_intersect(t_ray r, t_aabb bound, float *near,
	float *far)
{
	t_aabb_inter	b;
	float			tmin;
	float			tmax;

	b.tminx = bound[r.sign[0]].x * r.invdir.x - r.orig_div.x;
	b.tmaxx = bound[1 - r.sign[0]].x * r.invdir.x - r.orig_div.x;
	b.tminy = bound[r.sign[1]].y * r.invdir.y - r.orig_div.y;
	b.tmaxy = bound[1 - r.sign[1]].y * r.invdir.y - r.orig_div.y;
	b.tminz = bound[r.sign[2]].z * r.invdir.z - r.orig_div.z;
	b.tmaxz = bound[1 - r.sign[2]].z * r.invdir.z - r.orig_div.z;
	tmin = fmaxf(fmaxf(b.tminx, b.tminy), b.tminz);
	tmax = fminf(fminf(b.tmaxx, b.tmaxy), b.tmaxz);
	if (tmax < fmaxf(tmin, 0.0f))
		return (0);
	if (near != NULL)
		near[0] = tmin;
	if (far != NULL)
		far[0] = tmax;
	return (1);
}

FORCEINLINE
extern inline int	triangle_inter(t_ray r, t_triangle *tr, t_hit *hit)
{
	t_moller	m;

	m.r_cross_e2 = vec3f_cross(r.dir, tr->edge2);
	m.det = vec3f_dot(tr->edge1, m.r_cross_e2);
	if (fabsf(m.det) < EPSILON)
		return (0);
	m.invdet = 1.f / m.det;
	m.s = vec3f_sub(r.origin, tr->v0);
	hit->u = m.invdet * vec3f_dot(m.s, m.r_cross_e2);
	if (hit->u < -EPSILON || hit->u > 1.f + EPSILON)
		return (0);
	m.s_cross_e1 = vec3f_cross(m.s, tr->edge1);
	hit->v = m.invdet * vec3f_dot(r.dir, m.s_cross_e1);
	if (hit->v < -EPSILON || hit->u + hit->v > 1.f + EPSILON)
		return (0);
	hit->t = m.invdet * vec3f_dot(tr->edge2, m.s_cross_e1);
	if (hit->t <= EPSILON)
		return (0);
	hit->inter = vec3f_add(r.origin, vec3f_scale(r.dir, hit->t));
	hit->normal = tr->normal;
	return (1);
}

FORCEINLINE
extern inline void	sortf2(float *a, float *b)
{
	if (*a > *b)
		swap_float(a, b);
}

FORCEINLINE
extern inline int	solve_quadratic(t_equ arg, float roots[])
{
	float	disc;
	float	disc_sqrt;
	float	q;

	if (fabsf(arg.a) < (float)EPSILON)
	{
		if (fabsf(arg.b) < (float)EPSILON)
			return (0);
		roots[0] = -arg.c / arg.b;
		return (1);
	}
	disc = arg.b * arg.b - 4.0f * arg.a * arg.c;
	if (disc < 0.0f)
		return (0);
	disc_sqrt = sqrtf(disc);
	q = -0.5f * (arg.b + copysignf(disc_sqrt, arg.b));
	roots[0] = q / arg.a;
	roots[1] = arg.c / q;
	sortf2(&roots[0], &roots[1]);
	if (disc > EPSILON)
		return (2);
	return (1);
}

FORCEINLINE
extern inline void	sortf3(float *a, float *b, float *c)
{
	if (*a > *b)
		swap_float(a, b);
	if (*b > *c)
		swap_float(b, c);
	if (*a > *b)
		swap_float(a, b);
}

FORCEINLINE
extern inline void	sortf4(float *a, float *b, float *c, float *d)
{
	if (*a > *b)
		swap_float(a, b);
	if (*c > *d)
		swap_float(c, d);
	if (*a > *c)
		swap_float(a, c);
	if (*b > *d)
		swap_float(b, d);
	if (*b > *c)
		swap_float(b, c);
}

FORCEINLINE
extern inline int	solve_cubic(t_equ arg, float roots[])
{
	float	inv_a;
	float	third_b;
	float	b_square_3;
	float	half_q;
	float	third_p;
	float	p;
	float	q;
	float	disc;
	float	disc_sqrt;
	float	t1;
	float	t2;
	float	r;
	float	r2;
	float	phi;

	if (fabsf(arg.a) < EPSILON)
		return (solve_quadratic((t_equ){arg.b, arg.c, arg.d, 0, 0}, roots));
	inv_a = ft_rcpf(arg.a);
	arg.b *= inv_a;
	arg.c *= inv_a;
	arg.d *= inv_a;
	third_b = arg.b * RCP_3;
	b_square_3 = arg.b * third_b;
	p = arg.c - b_square_3;
	q = (2.0f * b_square_3 * arg.b * RCP_9)
		- (arg.b * arg.c * RCP_3) + arg.d;
	half_q = q * 0.5f;
	third_p = p * RCP_3;
	disc = half_q * half_q + third_p * third_p * third_p;
	if (disc > 0.0f)
	{
		disc_sqrt = sqrtf(disc);
		t1 = -half_q + disc_sqrt;
		t2 = -half_q - disc_sqrt;
		roots[0] = copysignf(cbrtf(fabsf(t1)), t1)
			+ copysignf(cbrtf(fabsf(t2)), t2) - third_b;
		return (1);
	}
	else
	{
		r = sqrtf(-third_p);
		phi = acosf(fmaxf(fminf(-half_q / (r * r * r), 1.0f), -1.0f));
		r2 = 2.0f * r;
		roots[0] = r2 * cosf(phi * RCP_3) - third_b;
		roots[1] = r2 * cosf((phi + 2.0f * (float)M_PI) * RCP_3) - third_b;
		roots[2] = r2 * cosf((phi + 4.0f * (float)M_PI) * RCP_3) - third_b;
		sortf3(&roots[0], &roots[1], &roots[2]);
		return (3);
	}
}

// perfect only norme left ?
FORCEINLINE
extern inline int	solve_quartic(t_equ arg, float roots[])
{
	float	inv_a;
	float	b2;
	float	b3;
	float	b4;
	float	p;
	float	q;
	float	r;
	float	z;
	float	u;
	float	disc;
	float	v;
	int		n;
	int		i;
	float	substitute;

	if (fabsf(arg.a) < EPSILON)
		return (solve_cubic((t_equ){arg.b, arg.c, arg.d, arg.e, 0}, roots));
	inv_a = ft_rcpf(arg.a);
	arg.b *= inv_a;
	arg.c *= inv_a;
	arg.d *= inv_a;
	arg.e *= inv_a;
	b2 = arg.b * arg.b;
	b3 = arg.b * b2;
	b4 = b2 * b2;
	p = arg.c - THREE_OVER_8 * b2; // c - ab2/8
	q = RCP_8 * b3 - (RCP_2 * arg.b * arg.c) + arg.d; // b3/8 - bc/2 + d
	r = -THREE_OVER_256 * b4 + RCP_16 * b2 * arg.c
		- RCP_4 * arg.b * arg.d + arg.e; // -3b4/256 + b2c/16 - bd/4 + e
	// z3 - z2p/2 - rz + (rp/2 - q2/8) one root;
	(void)solve_cubic((t_equ){1.0f, -RCP_2 * p, -r,
		-RCP_2 * r * p - RCP_8 * q * q, 0}, roots);
	z = roots[0];
	u = sqrtf(fmaxf(2.0f * z - p, 0.0f));
	disc = -4.0f * z * z + 4.0f * r + (q * q) / (u * u);
	if (fabsf(u) < EPSILON || fabsf(disc) < EPSILON)
		v = 0.0f;
	else
		v = -q / (2.0f * u);
	n = solve_quadratic((t_equ){1.0f, u, z - v, 0, 0}, roots);
	n += solve_quadratic((t_equ){1.0f, -u, z + v, 0, 0}, roots + n);
	substitute = arg.b * RCP_4;
	i = 0;
	while (i < n)
	{
		roots[i] -= substitute; // x = y - b/4
		i += 1;
	}
	if (n == 2)
		sortf2(&roots[0], &roots[1]);
	else if (n == 3)
		sortf3(&roots[0], &roots[1], &roots[2]);
	else if (n == 4)
		sortf4(&roots[0], &roots[1], &roots[2], &roots[3]);
	return (n);
}

FORCEINLINE
extern inline void	_sphere_inter(t_ray r, t_sphere *s, t_hit *hit, float t)
{
	const float	inv_pi = 1.0f / (float)M_PI;
	const float	inv2_pi = 0.5f * inv_pi;
	float		theta;
	float		phi;

	hit->t = t;
	hit->inter = vec3f_add(r.origin, vec3f_scale(r.dir, t));
	hit->normal = vec3f_scale(vec3f_sub(hit->inter, s->center), s->inv_r);
	theta = atan2f(hit->normal.z, hit->normal.x);
	phi = acosf(hit->normal.y);
	hit->u = (theta + (float)M_PI) * (inv2_pi);
	hit->v = phi * (inv_pi);
}

FORCEINLINE
extern inline int	sphere_inter(t_ray r, t_sphere *s, t_hit *hit)
{
	t_sphere_inter	m;

	m.L = vec3f_sub(r.origin, s->center);
	m.b = 2.0f * vec3f_dot(r.dir, m.L);
	m.c = vec3f_dot(m.L, m.L) - s->r_squared;
	if (solve_quadratic((t_equ){1, m.b, m.c, 0, 0},
		(float []){m.t0, m.t1}) == 0)
		return (0);
	if (m.t0 > EPSILON)
		m.t = m.t0;
	else if (m.t1 > EPSILON)
		m.t = m.t1;
	else
		m.t = -1.0f;
	if (m.t < -EPSILON)
		return (0);
	_sphere_inter(r, s, hit, m.t);
	return (1);
}

FORCEINLINE
extern inline int	_cylinder_inter(t_ray r, t_cylinder *cl, t_hit *hit,
	t_cylinder_inter m)
{
	const float	inv2_pi = 0.5f / (float)M_PI;

	m.inter = vec3f_add(r.origin, vec3f_scale(r.dir, m.t));
	m.y = vec3f_dot(vec3f_sub(m.inter, cl->p0), cl->axis);
	if (m.y < -EPSILON || m.y > cl->height + EPSILON)
		return (0);
	hit->inter = m.inter;
	hit->normal = vec3f_normalize(vec3f_sub(
				m.inter,
				vec3f_add(cl->p0, vec3f_scale(cl->axis, m.y))));
	hit->t = m.t;
	hit->u = (atan2f(hit->normal.z, hit->normal.x) + (float)M_PI) * inv2_pi;
	hit->v = m.y / cl->height;
	return (1);
}

FORCEINLINE
extern inline int	cylinder_inter(t_ray r, t_cylinder *cl, t_hit *hit)
{
	t_cylinder_inter	m;

	m.delta = vec3f_sub(r.origin, cl->p0);
	m.d = vec3f_sub(r.dir, vec3f_scale(cl->axis, vec3f_dot(r.dir, cl->axis)));
	m.delta_proj = vec3f_sub(m.delta, vec3f_scale(cl->axis, vec3f_dot(m.delta,
					cl->axis)));
	m.a = vec3f_dot(m.d, m.d);
	m.b = 2.0f * vec3f_dot(m.d, m.delta_proj);
	m.c = vec3f_dot(m.delta_proj, m.delta_proj) - cl->r_squared;
	if (solve_quadratic((t_equ){m.a, m.b, m.c, 0, 0},
		(float []){m.t0, m.t1}) == 0)
		return (0);
	if (m.t0 > EPSILON)
		m.t = m.t0;
	else if (m.t1 > EPSILON)
		m.t = m.t1;
	else
		m.t = -1.0f;
	if (m.t < -EPSILON)
		return (0);
	return (_cylinder_inter(r, cl, hit, m));
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
//etc... build mat from euler angles maybe

// rough start need to precompute a lot of things
FORCEINLINE
extern inline int	torus_inter(t_ray r, t_torus *t, t_hit *hit)
{
	t_ray	r_local;
	t_vec3f	p;
	float	m, k;
	t_equ	eq;
	float	roots[4];
	int		nroots;
	float	tmin;
	t_vec3f	hit_local, normal_local;
	float	tmp;

	r_local.origin = mat3x3_mulv(t->inv_basis, vec3f_sub(r.origin, t->center));
	r_local.dir    = mat3x3_mulv(t->inv_basis, r.dir);
	p = r_local.origin;
	m = vec3f_dot(r_local.dir, r_local.dir);
	k = vec3f_dot(p, p) + t->R * t->R - t->r * t->r;
	eq.a = m * m;
	eq.b = 4.0f * m * vec3f_dot(p, r_local.dir);
	eq.c = 2.0f * m * k + 4.0f * vec3f_dot(p, r_local.dir) * vec3f_dot(p, r_local.dir)
		- 4.0f * t->R * t->R * (r_local.dir.x * r_local.dir.x + r_local.dir.y * r_local.dir.y);
	eq.d = 4.0f * vec3f_dot(p, r_local.dir) * k
		- 8.0f * t->R * t->R * (p.x * r_local.dir.x + p.y * r_local.dir.y);
	eq.e = k * k - 4.0f * t->R * t->R * (p.x * p.x + p.y * p.y);
	nroots = solve_quartic(eq, roots);
	if (nroots == 0)
		return 0;
	tmin = -1.0f;
	for (int i = 0; i < nroots; ++i)
		if (roots[i] > EPSILON && (tmin < 0.0f || roots[i] < tmin))
			tmin = roots[i];
	if (tmin < 0.0f)
		return 0;
	hit_local = vec3f_add(r_local.origin, vec3f_scale(r_local.dir, tmin));
	p = hit_local;
	tmp = vec3f_dot(p, p) + t->R * t->R - t->r * t->r;
	normal_local.x = 4.0f * p.x * tmp - 8.0f * t->R * t->R * p.x;
	normal_local.y = 4.0f * p.y * tmp - 8.0f * t->R * t->R * p.y;
	normal_local.z = 4.0f * p.z * tmp;
	normal_local = vec3f_normalize(normal_local);
	hit->t = tmin;
	hit->inter = vec3f_add(t->center, mat3x3_mulv(t->basis, hit_local));
	hit->normal = vec3f_normalize(mat3x3_mulv(t->basis, normal_local));
	return 1;
}

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
