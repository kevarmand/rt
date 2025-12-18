/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 19:23:53 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/18 20:16:30 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../geometry.h"

void	build_geo_sphere(const t_scene *scene,
			const t_ray *ray,
			const t_sphere *sphere,
			t_hit *hit)
{
	const t_surface_map	*surface;
	t_vec3f				normal;
	t_bump_apply		bump;

	surface = &scene->surfaces[hit->surface_id];
	normal = vec3f_normalize(vec3f_sub(hit->point, sphere->center));
	hit->geo_normal = normal;
	if (surface_needs_uv(surface))
	{
		sphere_build_uv(sphere, hit);
		surface_apply_uv(surface, hit);
	}
	if (surface_needs_bump(surface))
	{
		bump.scene = scene;
		bump.surface = surface;
		sphere_build_tbn(normal, &bump.tangent, &bump.bitangent);
		bump.u = hit->u;
		bump.v = hit->v;
		bump_apply_normal(&bump, &normal);
	}
	hit->normal = normal;
	hit_faceforward(ray, hit);
}
