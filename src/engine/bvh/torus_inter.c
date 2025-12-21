/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   torus_inter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: norivier <norivier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 01:37:05 by norivier          #+#    #+#             */
/*   Updated: 2025/12/21 01:38:28 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bvh.h"
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
static t_vec3f	to_localf(t_vec3f p, t_vec3f ux, t_vec3f uy, t_vec3f uz)
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
extern inline int	torus_interf(t_ray r, t_torus *t, t_hit *hit)
{

	t_vec3f	ro;
	t_vec3f	rd;
	t_equ	eq;
	float	roots[4];
	float	g;
	float	h;
	float	i;
	float	j;
	float	k;
	float	l;
	int		nroots;

	ro = r.origin - t->center;
	rd = r.dir;
	t_vec3f	uy = t->normal;
	t_vec3f	ux,uz;
	frisvadf(uy, &ux, &uz);
	ro = to_localf(ro, ux, uy, uz);
	rd = to_localf(rd, ux, uy, uz);
	float	inv_r = 1.0 / t->r0;
	ro *= inv_r;
	float	r_scaled = t->r1 * inv_r;
	g = 4.0 * (rd.x * rd.x + rd.z * rd.z);
	h = 8.0 * (ro.x * rd.x + ro.z * rd.z);
	i = 4.0 * (ro.x * ro.x + ro.z * ro.z);
	j = 1.0;
	k = 2.0 * vec3f_dot(ro, rd);
	l = vec3f_dot(ro, ro) + 1.0 - r_scaled * r_scaled;
	eq.a = j * j;
	eq.b = 2.0 * j * k;
	eq.c = 2.0 * j * l + k * k - g;
	eq.d = 2.0 * k * l - h;
	eq.e = l * l - i;
	nroots = solve_quarticf(eq, roots);
	int		hit_happened = 0;
	for (int z = 0; z < nroots; ++z)
	{
		roots[z] *= t->r0;
		if (roots[z] > 1e-6 && roots[z] < hit->t)
		{
			hit->t = roots[z];
			hit_happened = 1;
		}
	}
	return (hit_happened);
}
