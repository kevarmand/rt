/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_surface_uv2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 22:09:45 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/17 22:47:17 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scene.h"
#include "vector.h"
#include <math.h>
#include "engine.h"
#include "scene.h"
#include <math.h>

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

void	init_surface_plane_cache(t_surface_map *surface,
		const t_plane *plane)
{
	surface->normal = plane->normal;
	surface_build_frame_from_normal(surface->normal,
		&surface->tang, &surface->bitang);
}

typedef struct s_tbn_ctx
{
	float	du1;
	float	dv1;
	float	du2;
	float	dv2;
	float	det;
}	t_tbn_ctx;

static void	triangle_build_tbn_cache(const t_triangle *triangle,
			t_surface_map *surface, t_vec3f normal)
{
	t_tbn_ctx	ctx;
	t_vec3f		*tangent;
	t_vec3f		*bitangent;

	tangent = &surface->tang;
	bitangent = &surface->bitang;
	ctx.du1 = surface->map_uv[2] - surface->map_uv[0];
	ctx.dv1 = surface->map_uv[3] - surface->map_uv[1];
	ctx.du2 = surface->map_uv[4] - surface->map_uv[0];
	ctx.dv2 = surface->map_uv[5] - surface->map_uv[1];
	ctx.det = ctx.du1 * ctx.dv2 - ctx.dv1 * ctx.du2;
	if (fabsf(ctx.det) < 0.000001f)
	{
		surface_build_frame_from_normal(normal, tangent, bitangent);
		return ;
	}
	*tangent = vec3f_scale(vec3f_sub(vec3f_scale(triangle->edge1, ctx.dv2),
				vec3f_scale(triangle->edge2, ctx.dv1)), 1.0f / ctx.det);
	*bitangent = vec3f_scale(vec3f_sub(vec3f_scale(triangle->edge2, ctx.du1),
				vec3f_scale(triangle->edge1, ctx.du2)), 1.0f / ctx.det);
	*tangent = vec3f_normalize(*tangent);
	*bitangent = vec3f_normalize(*bitangent);
}

void	init_surface_triangle_cache(t_surface_map *surface,
			const t_triangle *triangle)
{
	surface->normal = vec3f_normalize(vec3f_cross(triangle->edge1,
				triangle->edge2));
	triangle_build_tbn_cache(triangle, surface, surface->normal);
	surface->uv_jacobian_t[0] = 1.0f;
	surface->uv_jacobian_t[1] = 0.0f;
	surface->uv_jacobian_t[2] = 0.0f;
	surface->uv_jacobian_t[3] = 1.0f;
}
