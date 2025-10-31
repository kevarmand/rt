/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aabb_logic.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: norivier <norivier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 02:01:43 by norivier          #+#    #+#             */
/*   Updated: 2025/10/31 02:10:30 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "types.h"
#include "math.h"
#include "bvh.h"

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
	if (near != ((void *)0))
		near[0] = tmin;
	if (far != ((void *)0))
		far[0] = tmax;
	return (1);
}
