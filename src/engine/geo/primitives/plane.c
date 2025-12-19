/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 19:10:27 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/19 17:42:29 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../geometry.h"

void	build_geo_plane(const t_scene *scene,
			const t_ray *ray,
			const t_plane *plane,
			t_hit *hit)
{
	const t_surface	*surface;
	t_vec3f				normal;
	t_bump_apply		bump;

	surface = &scene->surfaces[hit->surface_id];
	normal = plane->normal;
	hit->geo_normal = normal;
	if (surface_needs_uv(surface))
		plane_build_uv(surface, hit);
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
