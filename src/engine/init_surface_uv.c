/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_surface_uv.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 16:31:16 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/14 16:02:12 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scene.h"
#include "vector.h"
#include <math.h>

static void	init_surface_uv_one(t_surface_map *surface, const t_primitive *primitive)
{
	float	det;

	if (primitive->type == PRIM_TRIANGLE)
		return ;
	det = surface->map_uv[0] * surface->map_uv[3]
		- surface->map_uv[1] * surface->map_uv[2];
	if (fabsf(det) < 1e-6f)
	{
		surface->map_uv[0] = 1.0f;
		surface->map_uv[1] = 0.0f;
		surface->map_uv[2] = 0.0f;
		surface->map_uv[3] = 1.0f;
		surface->map_uv[4] = 0.0f;
		surface->map_uv[5] = 0.0f;
	}
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
