/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prim_inter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 02:11:27 by norivier          #+#    #+#             */
/*   Updated: 2025/12/21 01:58:44 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bvh.h"
#include "scene.h"
#include "vector.h"

__attribute__((always_inline))
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

__attribute__((always_inline))
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

__attribute__((always_inline))
static inline int	cylinder_inter0(t_cylinder_inter_ctx *c, t_hit *hit,
		t_cylinder *cl)
{
	int		hit_happened;
	int		i;
	float	t;

	hit_happened = 0;
	i = 0;
	while (i < c->nroots)
	{
		if (c->roots[i] > 0.0f)
		{
			t = c->ro_dot + c->rd_dot * c->roots[i];
			if (t > 0.0f && t <= cl->height)
			{
				if (c->roots[i] < hit->t)
				{
					hit->t = c->roots[i];
					hit_happened = 1;
				}
			}
		}
		i += 1;
	}
	return (hit_happened);
}

__attribute__((always_inline))
extern inline int	cylinder_inter(t_ray r, t_cylinder *cl, t_hit *hit)
{
	t_vec3f					ro;
	t_vec3f					rd;
	t_equ					eq;
	t_cylinder_inter_ctx	c;

	ro = r.origin - cl->base;
	c.ro_dot = vec3f_dot(ro, cl->axis);
	ro = ro - cl->axis * c.ro_dot;
	c.rd_dot = vec3f_dot(r.dir, cl->axis);
	rd = r.dir - cl->axis * c.rd_dot;
	eq.a = vec3f_dot(rd, rd);
	eq.b = 2.0f * vec3f_dot(rd, ro);
	eq.c = vec3f_dot(ro, ro) - cl->r_squared;
	c.nroots = solve_quad(eq, c.roots);
	return (cylinder_inter0(&c, hit, cl));
}

__attribute__((always_inline))
extern inline int	prim_inter(t_ray r, t_primitive *p,
	t_hit *out, float tnear)
{
	t_vec3f	tmp_origin;
	float	bias;
	int		status;

	tmp_origin = r.origin;
	bias = 0.0f;
	if (tnear > 0.0f)
	{
		bias = tnear - 1.0f;
		out->t -= bias;
	}
	r.origin += bias * r.dir;
	status = 0;
	if (p->type == PRIM_TRIANGLE)
		status = triangle_inter(r, &p->tr, out);
	else if (p->type == PRIM_SPHERE)
		status = sphere_inter(r, &p->sp, out);
	else if (p->type == PRIM_CYLINDER)
		status = cylinder_inter(r, &p->cy, out);
	else if (p->type == PRIM_TORUS)
		status = torus_interf(r, &p->to, out);
	out->t += bias;
	r.origin = tmp_origin;
	return (status);
}
