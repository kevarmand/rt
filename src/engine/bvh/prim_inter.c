/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prim_inter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: norivier <norivier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 02:11:27 by norivier          #+#    #+#             */
/*   Updated: 2025/12/10 05:05:55 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <float.h>
#include <math.h>
#include "bvh.h"
#include "types.h"
#include "rt_math.h"
#include "vector.h"

#include <stdio.h>
// Not sure on what to return yet
FORCEINLINE
extern inline int	triangle_inter(t_ray r, t_triangle *tr, t_hit *hit)
{
	t_moller	m;
	t_vec3f		ro;
	t_vec3f		rd;

	// ro = mat3x3_mulv(tr->inv_basis, vec3f_sub(r.origin, tr->v0));
	// rd = mat3x3_mulv(tr->inv_basis, r.dir);
	ro = r.origin;
	rd = r.dir;
	m.r_cross_e2 = vec3f_cross(rd, tr->edge2);
	m.det = vec3f_dot(tr->edge1, m.r_cross_e2);
	if (fabsf(m.det) < EPSILON)
		return (0);
	m.invdet = 1.0f / m.det;
	m.s = vec3f_sub(ro, tr->v0);
	hit->u = m.invdet * vec3f_dot(m.s, m.r_cross_e2);
	if (hit->u < -EPSILON || hit->u > 1.0f + EPSILON)
		return (0);
	m.s_cross_e1 = vec3f_cross(m.s, tr->edge1);
	hit->v = m.invdet * vec3f_dot(rd, m.s_cross_e1);
	if (hit->v < -EPSILON || hit->u + hit->v > 1.0f + EPSILON)
		return (0);
	hit->t = m.invdet * vec3f_dot(tr->edge2, m.s_cross_e1);
	if (hit->t <= EPSILON)
		return (0);
	return (1);
}

// FORCEINLINE
// extern inline void	_sphere_inter(t_ray r, t_sphere *s, t_hit *hit)
// {
// 	const float	inv_pi = 1.0f / (float)M_PI;
// 	const float	inv2_pi = 0.5f * inv_pi;
// 	float		theta;
// 	float		phi;
// 	t_vec3f		n_local;
//
// 	hit->point = vec3f_add(r.origin, vec3f_scale(r.dir, hit->t));
// 	n_local = vec3f_scale(vec3f_sub(hit->point, s->center), s->inv_r);
// 	hit->point = vec3f_add(mat3x3_mulv(s->basis,
// 					vec3f_sub(hit->point, s->center)), s->center);
// 	hit->normal = vec3f_normalize(mat3x3_mulv(s->basis, n_local));
// 	theta = atan2f(hit->normal.z, hit->normal.x);
// 	phi = acosf(hit->normal.y);
// 	hit->u = (theta + (float)M_PI) * (inv2_pi);
// 	hit->v = phi * (inv_pi);
// }

void	print_vec3(t_vec3f vec);
FORCEINLINE
extern inline int	sphere_inter(t_ray r, t_sphere *s, t_hit *hit)
{
	t_vec3f			ro;
	t_vec3f			rd;
	t_equ			eq;
	float			roots[2];
	int				nroots;

	// ro = mat3x3_mulv(s->inv_basis, vec3f_sub(r.origin, s->center));
	// rd = mat3x3_mulv(s->inv_basis, r.dir);
	ro = vec3f_sub(r.origin, s->center);
	rd = r.dir;
	eq.a = 1.0f;
	eq.b = 2.0f * vec3f_dot(rd, ro);
	eq.c = vec3f_dot(ro, ro) - s->r_squared;
	nroots = solve_quad(eq, roots);
	if (nroots == 0)
		return (0);
	hit->t = -1.0f;
	if (roots[0] > EPSILON)
		hit->t = roots[0];
	else if (nroots > 1 && roots[1] > EPSILON)
		hit->t = roots[1];
	if (hit->t < -EPSILON)
		return (0);
	return (1);
}

FORCEINLINE
extern inline int	cap_inter(t_ray r, t_cylinder *cl, t_hit *hit, t_equ *eq)
{
	float	t_plane;
	t_vec3f	p_local;

	solve_quad(*eq, &t_plane);
	if (t_plane > EPSILON && t_plane < hit->t)
	{
		p_local = vec3f_add(r.origin, vec3f_scale(r.dir, t_plane));
		if (p_local.x * p_local.x + p_local.z * p_local.z
			<= cl->r_squared + EPSILON)
		{
			hit->t = t_plane;
			hit->point = p_local;
			return (1);
		}
	}
	return (0);
}

FORCEINLINE
extern inline int	cylinder_cap(t_ray r, t_cylinder *cl, t_hit *hit)
{
	int		hit_happened;
	t_equ	eq;

	eq.a = 0;
	eq.b = r.dir.y;
	eq.c = r.origin.y;
	hit_happened = 0;
	if (cap_inter(r, cl, hit, &eq))
	{
		hit_happened = 1;
		hit->normal = -cl->axis;
	}
	eq.c = r.origin.y - cl->height;
	if (cap_inter(r, cl, hit, &eq))
	{
		hit_happened = 1;
		hit->normal = cl->axis;
	}
	return (hit_happened);
}

FORCEINLINE
extern inline int	cylinder_inter(t_ray r, t_cylinder *cl, t_hit *hit)
{
	t_vec3f				ro;
	t_vec3f				rd;
	t_equ				eq;
	float				roots[2];
	int					nroots;
	t_vec3f				p_local;
	int					hit_happened;
	int					i;

	// ro = mat3x3_mulv(cl->inv_basis, vec3f_sub(r.origin, cl->p0));
	// rd = mat3x3_mulv(cl->inv_basis, r.dir);
	ro = r.origin;
	rd = r.dir;
	eq.a = rd.x * rd.x + rd.z * rd.z;
	eq.b = 2.0f * (ro.x * rd.x + ro.z * rd.z);
	eq.c = ro.x * ro.x + ro.z * ro.z - cl->r_squared;
	nroots = solve_quad(eq, roots);
	hit_happened = 0;
	i = 0;
	hit->t = FLT_MAX;
	while (i < nroots)
	{
		if (roots[i] > EPSILON)
		{
			p_local = vec3f_add(ro, vec3f_scale(rd, roots[i]));
			if (p_local.y > -EPSILON && p_local.y <= cl->height + EPSILON)
			{
				if (roots[i] < hit->t)
				{
					hit->t = roots[i];
					hit->point = p_local;
					hit->normal = vec3f_normalize((t_vec3f){hit->point.x, 0, hit->point.z});
					hit_happened = 1;
				}
			}
		}
		i += 1;
	}
	if (fabsf(rd.y) > EPSILON) {
		if (cylinder_cap(r, cl, hit))
			hit_happened = 1;
	}
	if (hit_happened == 0)
		return (0);
	return (1);
}

// Looks ok ?
FORCEINLINE
extern inline int	torus_inter(t_ray r, t_torus *t, t_hit *hit)
{
	t_ray	r_local;
	t_vec3f	p;
	float	m;
	float	k;
	t_equ	eq;
	float	roots[4];
	int		nroots;
	float	tmin;
	// t_vec3f	hit_local;
	// t_vec3f	normal_local;
	// float	tmp;
	float	p_dot_d;

	// r_local.origin = mat3x3_mulv(t->inv_basis, vec3f_sub(r.origin, t->center));
	// r_local.dir    = mat3x3_mulv(t->inv_basis, r.dir);
	r_local.origin = r.origin;
	r_local.dir = r.dir;
	p = r_local.origin;
	m = vec3f_dot(r_local.dir, r_local.dir);
	k = vec3f_dot(p, p) + t->R * t->R - t->r * t->r;
	p_dot_d = vec3f_dot(p, r_local.dir);
	eq.a = m * m;
	eq.b = 4.0f * m * p_dot_d;
	eq.c = 2.0f * m * k + 4.0f * p_dot_d * p_dot_d - 4.0f * t->R * t->R
		* (r_local.dir.x * r_local.dir.x + r_local.dir.y * r_local.dir.y);
	eq.d = 4.0f * p_dot_d * k
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
	// hit_local = vec3f_add(r_local.origin, vec3f_scale(r_local.dir, tmin));
	// p = hit_local;
	// tmp = vec3f_dot(p, p) + t->R * t->R - t->r * t->r;
	// normal_local.x = 4.0f * p.x * tmp - 8.0f * t->R * t->R * p.x;
	// normal_local.y = 4.0f * p.y * tmp - 8.0f * t->R * t->R * p.y;
	// normal_local.z = 4.0f * p.z * tmp;
	// normal_local = vec3f_normalize(normal_local);
	hit->t = tmin;
	// hit->point = vec3f_add(t->center, mat3x3_mulv(t->basis, hit_local));
	// hit->normal = vec3f_normalize(mat3x3_mulv(t->basis, normal_local)); //assuming orthonormal otherwise replace basis by transpose of invbasis
	return 1;
}

FORCEINLINE
extern inline int	prim_inter(t_ray r, t_primitive *p, t_hit *out)
{
	if (p->type == PRIM_TRIANGLE)
	{
		int	result;

		// p->tr.edge1 -= p->tr.v0;
		// p->tr.edge2 -= p->tr.v0;
		result = triangle_inter(r, &p->tr, out);
		// p->tr.edge1 += p->tr.v0;
		// p->tr.edge2 += p->tr.v0;
		return (result);
	}
	else if (p->type == PRIM_SPHERE)
		return (sphere_inter(r, &p->sp, out));
	else if (p->type == PRIM_CYLINDER)
		return (cylinder_inter(r, &p->cy, out));
	else if (p->type == PRIM_TORUS)
		return (torus_inter(r, &p->to, out));
	return (0);
}
