/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 19:26:07 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/19 17:42:29 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../geometry.h"

void	build_geo_triangle(const t_scene *scene,
			const t_ray *ray,
			const t_surface *surface,
			t_hit *hit)
{
	t_vec3f			normal;
	t_bump_apply	bump;

	normal = surface->normal;
	hit->geo_normal = normal;
	if (surface_needs_uv(surface))
		triangle_build_uv_bary(surface, hit->u, hit->v, hit);
	if (surface_needs_bump(surface))
	{
		bump.scene = scene;
		bump.surface = surface;
		bump.tangent = surface->tang;
		bump.bitangent = surface->bitang;
		bump.u = hit->u;
		bump.v = hit->v;
		bump_apply_normal(&bump, &normal);
	}
	hit->normal = normal;
	hit_faceforward(ray, hit);
}
