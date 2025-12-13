/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_build_geometry.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 17:32:19 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/13 19:56:26 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include "scene.h"
#include <math.h>

static void	hit_set_point_and_view(const t_ray *ray, t_hit *hit)
{
	t_vec3f	point;

	point = vec3f_add(ray->origin, vec3f_scale(ray->dir, hit->t));
	hit->point = point;
	hit->view_dir = vec3f_scale(ray->dir, -1.0f);
}

static void	hit_faceforward(const t_ray *ray, t_hit *hit)
{
	if (vec3f_dot(hit->normal, ray->dir) > 0.0f)
		hit->normal = vec3f_scale(hit->normal, -1.0f);
}

void	surface_apply_uv(const t_surface *surface, t_hit *hit)
{
	float	base_u;
	float	base_v;

	base_u = hit->u;
	base_v = hit->v;
	hit->u = surface->map_uv[0] * base_u
		+ surface->map_uv[1] * base_v
		+ surface->map_uv[4];
	hit->v = surface->map_uv[2] * base_u
		+ surface->map_uv[3] * base_v
		+ surface->map_uv[5];
}



void	build_geometry_plane(const t_scene *scene, t_hit *hit)
{
	const t_primitive	*plane;
	t_vec3f				normal;
	t_vec3f				axis;
	t_vec3f				tangent;
	t_vec3f				bitangent;
	t_vec3f				origin;
	t_vec3f				delta;

	plane = &scene->planes[hit->primitive_id];
	normal = plane->pl.normal;
	hit->normal = normal;
	axis = (t_vec3f){1.0f, 0.0f, 0.0f};
	if (fabsf(normal[0]) > 0.95f)
		axis = (t_vec3f){0.0f, 1.0f, 0.0f};
	tangent = vec3f_sub(axis, vec3f_scale(normal, vec3f_dot(axis, normal)));
	tangent = vec3f_normalize(tangent);
	bitangent = vec3f_normalize(vec3f_cross(normal, tangent));
	origin = vec3f_scale(normal, -plane->pl.d);
	delta = vec3f_sub(hit->point, origin);
	hit->u = vec3f_dot(delta, tangent);
	hit->v = vec3f_dot(delta, bitangent);
}

static void	hit_build_plane(const t_scene *scene, t_hit *hit)
{
	const t_primitive	*plane_prim;

	plane_prim = &scene->planes[hit->primitive_id];
	hit->surface_id = plane_prim->surface_id;
	hit->material_id = plane_prim->material_id;
	build_geometry_plane(scene, hit);
	surface_apply_uv(&scene->surfaces[hit->surface_id], hit);
	
}

static float	clamp_unit(float value)
{
	if (value > 1.0f)
		return (1.0f);
	if (value < -1.0f)
		return (-1.0f);
	return (value);
}

static void	build_primitive_sphere(const t_sphere *sphere,
				t_hit *hit, t_vec3f *normal)
{
	t_vec3f	local;
	float	phi;
	float	theta;

	local = vec3f_sub(hit->point, sphere->center);
	*normal = vec3f_normalize(local);
	phi = atan2f(local[2], local[0]);
	if (phi < 0.0f)
		phi += 2.0f * (float)M_PI;
	theta = acosf(clamp_unit(local[1] / sphere->radius));
	hit->u = phi / (2.0f * (float)M_PI);
	hit->v = theta / (float)M_PI;
}
static t_vec3f	cyl_ref_axis(t_vec3f axis)
{
	if (fabsf(axis[0]) > 0.95f)
		return ((t_vec3f){0.0f, 1.0f, 0.0f});
	return ((t_vec3f){1.0f, 0.0f, 0.0f});
}

static void	cyl_build_frame(t_vec3f axis, t_vec3f *tangent, t_vec3f *bitangent)
{
	t_vec3f	ref;

	ref = cyl_ref_axis(axis);
	*tangent = vec3f_normalize(vec3f_cross(axis, ref));
	*bitangent = vec3f_cross(axis, *tangent);
}

static float	cyl_angle_u(t_vec3f radial_unit, t_vec3f tangent, t_vec3f bitangent)
{
	float	x;
	float	y;
	float	a;

	x = vec3f_dot(radial_unit, tangent);
	y = vec3f_dot(radial_unit, bitangent);
	a = atan2f(y, x);
	if (a < 0.0f)
		a += 2.0f * (float)M_PI;
	return (a / (2.0f * (float)M_PI));
}

static void	build_primitive_cylinder(const t_cylinder *cylinder,
				t_hit *hit, t_vec3f *normal)
{
	t_vec3f	base_to_point;
	t_vec3f	proj_point;
	t_vec3f	radial;
	t_vec3f	tangent;
	t_vec3f	bitangent;
	float	height_pos;

	base_to_point = vec3f_sub(hit->point, cylinder->base);
	height_pos = vec3f_dot(base_to_point, cylinder->axis);
	proj_point = vec3f_add(cylinder->base, vec3f_scale(cylinder->axis, height_pos));
	radial = vec3f_sub(hit->point, proj_point);
	*normal = vec3f_normalize(radial);
	cyl_build_frame(cylinder->axis, &tangent, &bitangent);
	hit->u = cyl_angle_u(*normal, tangent, bitangent);
	hit->v = height_pos / cylinder->height;
}

static void	build_primitive_triangle(const t_primitive *primitive,
				t_hit *hit, const t_surface *surface)
{
	const t_triangle	*tria;

	tria = &primitive->tr;

	hit->normal = vec3f_cross(tria->edge1, tria->edge2);
	hit->normal = vec3f_normalize(hit->normal);
	// coordonnées barycentriques ( on el fait apres  :)
	hit->u = 0.0f;
	hit->v = 0.0f;
}


static void	hit_build_primitive(const t_scene *scene, t_hit *hit)
{
	const t_primitive	*primitive;
	const t_surface		*surface;

	primitive = &scene->primitives[hit->primitive_id];
	hit->surface_id = primitive->surface_id;
	hit->material_id = primitive->material_id;
	surface = &scene->surfaces[hit->surface_id];
	if (primitive->type == PRIM_SPHERE)
		build_primitive_sphere(&primitive->sp, hit, &hit->normal);
	else if (primitive->type == PRIM_CYLINDER)
		build_primitive_cylinder(&primitive->cy, hit, &hit->normal);
	else if (primitive->type == PRIM_TRIANGLE)
		build_primitive_triangle(primitive, hit, surface);
	if (primitive->type != PRIM_TRIANGLE)
		surface_apply_uv(surface, hit);
}


void	hit_build_geometry(const t_scene *scene, const t_ray *ray, t_hit *hit)
{
	hit->normal = (t_vec3f){0.0f, 0.0f, 0.0f};
	hit_set_point_and_view(ray, hit);
	if (hit->kind == HIT_PLANE)
		hit_build_plane(scene, hit);
	else if (hit->kind == HIT_PRIMITIVE)
		hit_build_primitive(scene, hit);
	hit->albedo = (t_vec3f){1.0f, 0.5f, 0.5f};
	hit_faceforward(ray, hit);
	if (hit->normal[0] == 0.0f && hit->normal[1] == 0.0f && hit->normal[2] == 0.0f)
		printf("normal pas build dans hit_build_geometry\n");
}
