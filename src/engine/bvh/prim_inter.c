/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prim_inter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: norivier <norivier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 02:11:27 by norivier          #+#    #+#             */
/*   Updated: 2025/12/15 01:24:57 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <assert.h>
#include <float.h>
#include <math.h>
#include "bvh.h"
#include "engine.h"
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

	ro = r.origin;
	rd = r.dir;
	m.r_cross_e2 = vec3f_cross(rd, tr->edge2);
	m.det = vec3f_dot(tr->edge1, m.r_cross_e2);
	if (m.det == 0.0f)
		return (0);
	m.invdet = 1.0f / m.det;
	m.s = vec3f_sub(ro, tr->v0);
	hit->u = m.invdet * vec3f_dot(m.s, m.r_cross_e2);
	if (hit->u < 0.0f || hit->u > 1.0f)
		return (0);
	m.s_cross_e1 = vec3f_cross(m.s, tr->edge1);
	hit->v = m.invdet * vec3f_dot(rd, m.s_cross_e1);
	if (hit->v < 0.0f || hit->u + hit->v > 1.0f)
		return (0);
	hit->t = m.invdet * vec3f_dot(tr->edge2, m.s_cross_e1);
	return (hit->t > 0.0f);
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

FORCEINLINE
extern inline int	sphere_inter(t_ray r, t_sphere *s, t_hit *hit)
{
	t_vec3f			ro;
	t_vec3f			rd;
	t_equ			eq;
	float			roots[2];
	int				nroots;

	ro = vec3f_sub(r.origin, s->center);
	rd = r.dir;
	eq.a = 1.0f;
	eq.b = 2.0f * vec3f_dot(rd, ro);
	eq.c = vec3f_dot(ro, ro) - s->r_squared;
	nroots = solve_quad(eq, roots);
	if (nroots == 0)
		return (0);
	hit->t = -1.0f;
	if (roots[0] > 0.0f)
		hit->t = roots[0];
	else if (nroots > 1 && roots[1] > 0.0f)
		hit->t = roots[1];
	else
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
	float				ro_dot;
	t_vec3f				rd;
	float				rd_dot;
	t_equ				eq;
	float				roots[2];
	int					nroots;
	float				t;
	int					hit_happened;
	int					i;

	ro = r.origin - cl->base;
	ro_dot = vec3f_dot(ro, cl->axis);
	ro = ro - cl->axis * ro_dot;
	rd_dot = vec3f_dot(r.dir, cl->axis);
	rd = r.dir - cl->axis * rd_dot;
	eq.a = vec3f_dot(rd, rd);
	eq.b = 2.0f * vec3f_dot(rd, ro);
	eq.c = vec3f_dot(ro, ro) - cl->r_squared;
	nroots = solve_quad(eq, roots);
	hit_happened = 0;
	i = 0;
	while (i < nroots)
	{
		if (roots[i] > 0.0f)
		{
			t = ro_dot + rd_dot * roots[i];
			if (t > 0.0f && t <= cl->height)
			{
				if (roots[i] < hit->t)
				{
					hit->t = roots[i];
					hit_happened = 1;
				}
			}
		}
		i += 1;
	}
	return (hit_happened);
}

void	frisvad(const t_vec3f axis, t_vec3f *b1, t_vec3f *b2)
{
	float	a;
	float	b;

	if (axis.y < -0.9999999f)
	{
		*b1 = (t_vec3f){0.0f, 0.0f, -1.0f, 0.0f};
		*b2 = (t_vec3f){-1.0f, 0.0f, 0.0f, 0.0f};
		return ;
	}
	a = ft_rcpf(1.0f + axis.y);
	b = -axis.x * axis.z * a;
	*b1 = (t_vec3f){1.0f - axis.x * axis.x * a, -axis.x, b, 0.0f};
	*b2 = (t_vec3f){b, -axis.z, 1.0f - axis.z * axis.z * a, 0.0f};
}

t_vec3f	to_local(t_vec3f p, t_vec3f ux, t_vec3f uy, t_vec3f uz)
{
	return ((t_vec3f){
		vec3f_dot(p, ux),
		vec3f_dot(p, uy),
		vec3f_dot(p, uz),
	});
}

t_vec3f	from_local(t_vec3f p, t_vec3f ux, t_vec3f uy, t_vec3f uz)
{
	return ((t_vec3f){
		p.x * ux.x + p.y * uy.x + p.z * uz.x,
		p.x * ux.y + p.y * uy.y + p.z * uz.y,
		p.x * ux.z + p.y * uy.z + p.z * uz.z,
	});
}

void	build_normal_torus(t_torus *t, t_vec3f *point, t_vec3f *normal)
{
	float	a;

	a = 1.0 - (t->R / sqrtf(point->x * point->x + point->z * point->z));
	*normal = vec3f_normalize((t_vec3f){a * point->x, point->y, a * point->z});
}

int	inside_torus(t_torus *t, t_vec3f *point)
{
	const float	t2 = t->R - sqrtf(point->x * point->x + point->z * point->z);
	const float	f = t2 * t2 + point->y * point->y - t->r_square;
	return (f <= EPSILON);
}

// Looks ok ?
FORCEINLINE
extern inline int	torus_inter(t_ray r, t_torus *t, t_hit *hit)
{
	t_vec3f	ro;
	t_vec3f	rd;
	t_cequ	eq;
	complex double	croots[4];
	double	roots[4];
	float	g;
	float	h;
	float	i;
	float	j;
	float	k;
	float	l;
	int		nroots;

	ro = vec3f_sub(r.origin, t->center);
	rd = r.dir;
	t_vec3f	uy = vec3f_normalize(t->normal);
	t_vec3f	ux,uz;
	frisvad(uy, &ux, &uz);
	t_vec3f	tmp;
	tmp = from_local(to_local(ro, ux, uy, uz), ux, uy, uz);
	assert(fabsf(tmp.x - ro.x) <= 1e-3f);
	assert(fabsf(tmp.z - ro.y) <= 1e-3f);
	assert(fabsf(tmp.z - ro.z) <= 1e-3f);
	ro = to_local(ro, ux, uy, uz);
	rd = to_local(rd, ux, uy, uz);
	g = 4 * t->R_square * (rd.x * rd.x + rd.z * rd.z);
	h = 8 * t->R_square * (ro.x * rd.x + ro.z * rd.z);
	i = 4 * t->R_square * (ro.x * ro.x + ro.z * ro.z);
	j = vec3f_dot(rd, rd);
	k = 2 * vec3f_dot(ro, rd);
	l = vec3f_dot(ro, ro) + (t->R_square - t->r_square);
	eq.a = j * j;
	eq.b = 2.0 * j * k;
	eq.c = 2.0 * j * l + k * k - g;
	eq.d = 2.0 * k * l - h;
	eq.e = l * l - i;
	nroots = csolve_quartic(eq, croots);
	nroots = filter_real_numbers(nroots, croots, roots);
	t_vec3f	hit_point_local;
	t_vec3f	hit_point_world;
	t_vec3f	normal;
	float	t_world;
	int		hit_happened = 0;
	for (int z = 0; z < nroots; ++z)
	{
		hit_point_local = ro + rd * roots[z];
		build_normal_torus(t, &hit_point_local, &normal);
		if (vec3f_dot(normal, rd) > 0.0f)
			continue ;
		hit_point_world = from_local(hit_point_local, ux, uy, uz) + t->center;
		t_world = vec3f_dot(hit_point_world - r.origin, r.dir);
		if (t_world > 1e-4f && t_world < hit->t)
		{
			hit->t = t_world;
			hit_happened = 1;
		}
	}
	return (hit_happened);
}

FORCEINLINE
extern inline int	prim_inter(t_ray r, t_primitive *p, t_hit *out)
{
	if (p->type == PRIM_TRIANGLE)
		return (triangle_inter(r, &p->tr, out));
	else if (p->type == PRIM_SPHERE)
		return (sphere_inter(r, &p->sp, out));
	else if (p->type == PRIM_CYLINDER)
		return (cylinder_inter(r, &p->cy, out));
	else if (p->type == PRIM_TORUS)
		return (torus_inter(r, &p->to, out));
	return (0);
}
