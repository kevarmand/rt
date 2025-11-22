/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_build_geometry.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 17:24:31 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/22 17:52:58 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include "scene.h"

static void	build_normal_plane(const t_plane *plane, t_vec3f *normal)
{
	*normal = plane->normal;
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

void	hit_build_geometry(const t_scene *scene,
			const t_ray *ray, t_hit *hit)
{
	const t_primitive	*primitive;
	t_vec3f				point;
	t_vec3f				normal;

	point = vec3f_add(ray->origin, vec3f_scale(ray->dir, hit->t));
	hit->point = point;
	if (hit->kind == HIT_PLANE)
		build_normal_plane(&scene->planes[hit->primitive_id].pl, &normal);
	else if (hit->kind == HIT_PRIMITIVE)
	{
		primitive = &scene->primitives[hit->primitive_id];
		build_normal_primitive(primitive, hit, &normal);
	}
	hit->normal = normal;
	if (vec3f_dot(hit->normal, ray->dir) > 0.0f)
		hit->normal = vec3f_scale(hit->normal, -1.0f);
}
