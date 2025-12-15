/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_surface_uv.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 16:31:16 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/15 19:50:23 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scene.h"
#include "vector.h"
#include <math.h>
#include "engine.h"
#include "scene.h"
#include <math.h>

static void	surface_set_affine_identity(t_surface_map *surface)
{
	surface->map_uv[0] = 1.0f;
	surface->map_uv[1] = 0.0f;
	surface->map_uv[2] = 0.0f;
	surface->map_uv[3] = 1.0f;
	surface->map_uv[4] = 0.0f;
	surface->map_uv[5] = 0.0f;
	surface->uv_jacobian_t[0] = 1.0f;
	surface->uv_jacobian_t[1] = 0.0f;
	surface->uv_jacobian_t[2] = 0.0f;
	surface->uv_jacobian_t[3] = 1.0f;
}

static void	surface_set_uv_jacobian_t(t_surface_map *surface)
{
	surface->uv_jacobian_t[0] = surface->map_uv[0];
	surface->uv_jacobian_t[1] = surface->map_uv[2];
	surface->uv_jacobian_t[2] = surface->map_uv[1];
	surface->uv_jacobian_t[3] = surface->map_uv[3];
}

static void	init_surface_uv_affine(t_surface_map *surface)
{
	float	det;

	det = surface->map_uv[0] * surface->map_uv[3]
		- surface->map_uv[1] * surface->map_uv[2];
	if (fabsf(det) < 1e-6f)
	{
		surface_set_affine_identity(surface);
		return ;
	}
	surface_set_uv_jacobian_t(surface);
}

static void	surface_build_frame_from_normal(t_vec3f normal,
			t_vec3f *tangent, t_vec3f *bitangent)
{
	t_vec3f	ref_axis;

	ref_axis = (t_vec3f){0.0f, 1.0f, 0.0f};
	if (fabsf(normal.y) > 0.999f)
		ref_axis = (t_vec3f){1.0f, 0.0f, 0.0f};
	*tangent = vec3f_cross(ref_axis, normal);
	*tangent = vec3f_normalize(*tangent);
	*bitangent = vec3f_cross(normal, *tangent);
	*bitangent = vec3f_normalize(*bitangent);
}

static void	init_surface_plane_cache(t_surface_map *surface, const t_plane *plane)
{
	surface->normal = plane->normal;
	surface_build_frame_from_normal(surface->normal, &surface->tang, &surface->bitang);
}

static void	triangle_build_tbn_cache(const t_triangle *triangle,
			const t_surface_map *surface, t_vec3f normal,
			t_vec3f *tangent, t_vec3f *bitangent)
{
	float	du1;
	float	dv1;
	float	du2;
	float	dv2;
	float	det;

	du1 = surface->map_uv[2] - surface->map_uv[0];
	dv1 = surface->map_uv[3] - surface->map_uv[1];
	du2 = surface->map_uv[4] - surface->map_uv[0];
	dv2 = surface->map_uv[5] - surface->map_uv[1];
	det = du1 * dv2 - dv1 * du2;
	if (fabsf(det) < 1e-8f)
	{
		surface_build_frame_from_normal(normal, tangent, bitangent);
		return ;
	}
	*tangent = vec3f_scale(vec3f_sub(vec3f_scale(triangle->edge1, dv2),
				vec3f_scale(triangle->edge2, dv1)), 1.0f / det);
	*bitangent = vec3f_scale(vec3f_sub(vec3f_scale(triangle->edge2, du1),
				vec3f_scale(triangle->edge1, du2)), 1.0f / det);
	*tangent = vec3f_normalize(*tangent);
	*bitangent = vec3f_normalize(*bitangent);
}


static void	init_surface_triangle_cache(t_surface_map *surface,
			const t_triangle *triangle)
{
	surface->normal = vec3f_normalize(vec3f_cross(triangle->edge1, triangle->edge2));
	triangle_build_tbn_cache(triangle, surface, surface->normal,
		&surface->tang, &surface->bitang);
	surface->uv_jacobian_t[0] = 1.0f;
	surface->uv_jacobian_t[1] = 0.0f;
	surface->uv_jacobian_t[2] = 0.0f;
	surface->uv_jacobian_t[3] = 1.0f;
}

static void	init_surface_uv_one(t_surface_map *surface, const t_primitive *primitive)
{
	if (primitive->type == PRIM_TRIANGLE)
	{
		init_surface_triangle_cache(surface, &primitive->tr);
		return ;
	}
	init_surface_uv_affine(surface);
	if (primitive->type == PRIM_PLANE)
		init_surface_plane_cache(surface, &primitive->pl);
}

void	init_surface_uv(t_scene *scene)
{
	int			index;
	t_primitive	*primitive;
	t_surface_map	*surface;

	index = 0;
	while (index < scene->plane_count)
	{
		primitive = &scene->planes[index++];
		surface = &scene->surfaces[primitive->surface_id];
		init_surface_uv_one(surface, primitive);
	}
	index = 0;
	while (index < scene->primitive_count)
	{
		primitive = &scene->primitives[index++];
		surface = &scene->surfaces[primitive->surface_id];
		init_surface_uv_one(surface, primitive);
	}
}
