/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   torus.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 19:54:50 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 01:11:45 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../geometry.h"

static void	torusinette(t_bump_apply *bump, t_hit *hit,
			const t_scene *scene, const t_surface_map *surface)
{
	bump->scene = scene;
	bump->surface = surface;
	bump->u = hit->u;
	bump->v = hit->v;
}

void	build_geo_torus(const t_scene *scene,
			const t_ray *ray,
			const t_torus *torus,
			t_hit *hit)
{
	const t_surface_map	*surface;
	t_vec3f				p;
	t_vec3f				radial;
	t_vec3f				normal;
	t_bump_apply		bump;

	surface = &scene->surfaces[hit->surface_id];
	p = vec3f_sub(hit->point, torus->center);
	radial = torus_compute_radial(torus, p);
	normal = vec3f_normalize(vec3f_sub(p, vec3f_scale(radial, torus->r0)));
	hit->geo_normal = normal;
	if (surface_needs_uv(surface))
	{
		torus_build_uv(torus, p, radial, hit);
		surface_apply_uv(surface, hit);
	}
	if (surface_needs_bump(surface))
	{
		torusinette(&bump, hit, scene, surface);
		torus_set_tbn(torus, radial, normal, &bump);
		bump_apply_normal(&bump, &normal);
	}
	hit->normal = normal;
	hit_faceforward(ray, hit);
}
