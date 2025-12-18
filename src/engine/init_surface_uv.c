/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_surface_uv.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 16:31:16 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/17 22:50:36 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scene.h"
#include "vector.h"
#include <math.h>
#include "engine.h"
#include "scene.h"
#include <math.h>

void	init_surface_triangle_cache(t_surface_map *surface,
			const t_triangle *triangle);
void	init_surface_plane_cache(t_surface_map *surface,
		const t_plane *plane);

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
	if (fabsf(det) < 0.000001f)
	{
		surface_set_affine_identity(surface);
		return ;
	}
	surface_set_uv_jacobian_t(surface);
}

static void	init_surface_uv_one(t_surface_map *surface,
	const t_primitive *primitive)
{
	init_surface_uv_affine(surface);
	if (primitive->type == PRIM_TRIANGLE)
	{
		init_surface_triangle_cache(surface, &primitive->tr);
		return ;
	}
	if (primitive->type == PRIM_PLANE)
		init_surface_plane_cache(surface, &primitive->pl);
}

void	init_surface_uv(t_scene *scene)
{
	int				index;
	t_primitive		*primitive;
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
