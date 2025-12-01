/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_build_geometry.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 17:24:31 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/01 19:16:13 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include "scene.h"
#include <stdio.h>
static void	build_geometry_plane(const t_scene *scene, t_hit *hit)
{
	t_primitive	*prim;
	t_surface	*surf;
	t_vec3f		origin;
	t_vec3f		delta;

	prim = &scene->planes[hit->primitive_id];
	surf = &scene->surfaces[prim->surface_id];
	hit->surface_id = prim->surface_id;
	hit->material_id = prim->material_id;
	hit->normal = prim->pl.normal;
	origin = vec3f_scale(prim->pl.normal, -prim->pl.d);
	delta = vec3f_sub(hit->point, origin);
	hit->u = vec3f_dot(delta, (t_vec3f){surf->map_uv[0],
			surf->map_uv[1], surf->map_uv[2]}) / surf->scale_u;
	hit->v = vec3f_dot(delta, (t_vec3f){surf->map_uv[3],
			surf->map_uv[4], surf->map_uv[5]}) / surf->scale_v;
}


static void	build_normal_sphere(const t_sphere *sphere,
			const t_hit *hit, t_vec3f *normal)
{
	t_vec3f	diff;

	diff = vec3f_sub(hit->point, sphere->center);
	*normal = vec3f_normalize(diff);
}

static void	build_normal_cylinder(const t_cylinder *cylinder,
			const t_hit *hit, t_vec3f *normal)
{
	t_vec3f	base_to_point;
	t_vec3f	proj_point;
	t_vec3f	radial;
	float	height_pos;

	base_to_point = vec3f_sub(hit->point, cylinder->base);
	height_pos = vec3f_dot(base_to_point, cylinder->axis);
	proj_point = vec3f_add(cylinder->base,
			vec3f_scale(cylinder->axis, height_pos));
	radial = vec3f_sub(hit->point, proj_point);
	*normal = vec3f_normalize(radial);
}

static void	build_normal_triangle(const t_triangle *triangle,
			t_vec3f *normal)
{
	t_vec3f	cross;

	cross = vec3f_cross(triangle->edge1, triangle->edge2);
	*normal = vec3f_normalize(cross);
}

static void	build_normal_primitive(const t_primitive *primitive,
			const t_hit *hit, t_vec3f *normal)
{
	if (primitive->type == PRIM_SPHERE)
		build_normal_sphere(&primitive->sp, hit, normal);
	else if (primitive->type == PRIM_CYLINDER)
		build_normal_cylinder(&primitive->cy, hit, normal);
	else if (primitive->type == PRIM_TRIANGLE)
		build_normal_triangle(&primitive->tr, normal);
}
#include <stdio.h>
void	hit_build_geometry(const t_scene *scene,
			const t_ray *ray, t_hit *hit)
{
	const t_primitive	*primitive;
	t_vec3f				point;
	t_vec3f				normal;

	normal = (t_vec3f){0.0f, 0.0f, 0.0f};
	point = vec3f_add(ray->origin, vec3f_scale(ray->dir, hit->t));
	hit->point = point;
	if (hit->kind == HIT_PLANE)
	{
		hit->surface_id = scene->planes[hit->primitive_id].surface_id;
		hit->material_id = scene->planes[hit->primitive_id].material_id;
		build_geometry_plane(scene, hit);
		normal = scene->planes[hit->primitive_id].pl.normal;
	}
	else if (hit->kind == HIT_PRIMITIVE)
	{
		hit->surface_id = scene->primitives[hit->primitive_id].surface_id;
		hit->material_id = scene->primitives[hit->primitive_id].material_id;
		primitive = &scene->primitives[hit->primitive_id];
		build_normal_primitive(primitive, hit, &normal);
		hit->albedo = scene->surfaces[hit->surface_id].color;
	}
	else
		printf("ca pue du cul dans hit_build_geometry\n");

	hit->view_dir = vec3f_scale(ray->dir, -1.0f);
	hit->normal = normal;
	if (vec3f_dot(hit->normal, ray->dir) > 0.0f)
	 	hit->normal = vec3f_scale(hit->normal, -1.0f);
}
