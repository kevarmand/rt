/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_build_geometry.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 19:09:30 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/19 17:41:48 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "geometry.h"

void	hit_build_geometry(const t_scene *scene, const t_ray *ray, t_hit *hit)
{
	const t_primitive	*primitive;
	const t_surface	*surface;

	hit->point = vec3f_add(ray->origin, vec3f_scale(ray->dir, hit->t));
	hit->view_dir = vec3f_scale(ray->dir, -1.0f);
	if (hit->kind == HIT_PLANE)
	{
		primitive = &scene->planes[hit->primitive_id];
		surface = &scene->surfaces[primitive->surface_id];
		hit->surface_id = primitive->surface_id;
		hit->material_id = primitive->material_id;
		build_geo_plane(scene, ray, &primitive->pl, hit);
		return ;
	}
	primitive = &scene->primitives[hit->primitive_id];
	surface = &scene->surfaces[primitive->surface_id];
	hit->surface_id = primitive->surface_id;
	hit->material_id = primitive->material_id;
	if (primitive->type == PRIM_SPHERE)
		build_geo_sphere(scene, ray, &primitive->sp, hit);
	else if (primitive->type == PRIM_CYLINDER)
		build_geo_cylinder(scene, ray, &primitive->cy, hit);
	else if (primitive->type == PRIM_TRIANGLE)
		build_geo_triangle(scene, ray, surface, hit);
}
