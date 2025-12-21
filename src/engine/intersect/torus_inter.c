/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   torus_inter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: norivier <norivier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 01:37:05 by norivier          #+#    #+#             */
/*   Updated: 2025/12/21 03:18:27 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bvh.h"
#include "engine.h"
#include "scene.h"
#include "vector.h"

__attribute__((always_inline))
static void	frisvadf(const t_vec3f axis, t_vec3f *b1, t_vec3f *b2)
{
	float	a;
	float	b;

	if (axis.y < -0.9999999)
	{
		*b1 = (t_vec3f){0.0, 0.0, -1.0, 0.0};
		*b2 = (t_vec3f){-1.0, 0.0, 0.0, 0.0};
		return ;
	}
	a = 1 / (1.0 + axis.y);
	b = -axis.x * axis.z * a;
	*b1 = (t_vec3f){1.0 - axis.x * axis.x * a, -axis.x, b, 0.0};
	*b2 = (t_vec3f){b, -axis.z, 1.0 - axis.z * axis.z * a, 0.0};
}

__attribute__((always_inline))
static inline t_vec3f	to_localf(t_vec3f p, t_vec3f ux,
		t_vec3f uy, t_vec3f uz)
{
	return ((t_vec3f){
		vec3f_dot(p, ux),
		vec3f_dot(p, uy),
		vec3f_dot(p, uz),
	});
}

// static t_vec3f	from_localf(t_vec3f p, t_vec3f ux, t_vec3f uy, t_vec3f uz)
// {
// 	return ((t_vec3f){
// 		p.x * ux.x + p.y * uy.x + p.z * uz.x,
// 		p.x * ux.y + p.y * uy.y + p.z * uz.y,
// 		p.x * ux.z + p.y * uy.z + p.z * uz.z,
// 	});
// }

__attribute__((always_inline))
static inline int	torus_interf0(t_torus_interf_ctx *c, t_torus *t,
		t_hit *hit)
{
	int		hit_happened;
	int		i;

	hit_happened = 0;
	i = 0;
	while (i < c->nroots)
	{
		c->roots[i] *= t->r0;
		if (c->roots[i] > 1e-6 && c->roots[i] < hit->t)
		{
			hit->t = c->roots[i];
			hit_happened = 1;
		}
		i += 1;
	}
	return (hit_happened);
}

__attribute__((always_inline))
extern inline int	torus_interf(t_ray r, t_torus *t, t_hit *hit)
{
	t_vec3f				ro;
	t_vec3f				rd;
	float				k;
	float				l;
	t_torus_interf_ctx	c;

	c.uy = t->normal;
	frisvadf(c.uy, &c.ux, &c.uz);
	ro = to_localf(r.origin - t->center, c.ux, c.uy, c.uz);
	rd = to_localf(r.dir, c.ux, c.uy, c.uz);
	ro *= t->inv_r0;
	k = 2.0 * vec3f_dot(ro, rd);
	l = vec3f_dot(ro, ro) + 1.0 - t->r1_scaled * t->r1_scaled;
	c.eq.a = 1.0f;
	c.eq.b = 2.0 * k;
	c.eq.c = 2.0 * l + k * k - 4.0 * (rd.x * rd.x + rd.z * rd.z);
	c.eq.d = 2.0 * k * l - 8.0 * (ro.x * rd.x + ro.z * rd.z);
	c.eq.e = l * l - 4.0 * (ro.x * ro.x + ro.z * ro.z);
	c.nroots = solve_quarticf(c.eq, c.roots);
	return (torus_interf0(&c, t, hit));
}
