/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 19:24:56 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/19 17:42:29 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../geometry.h"

static t_vec3f	cylinder_normal_raw(const t_cylinder *cylinder, t_vec3f point)
{
	t_vec3f	base_to_point;
	float	height_pos;
	t_vec3f	proj_point;

	base_to_point = vec3f_sub(point, cylinder->base);
	height_pos = vec3f_dot(base_to_point, cylinder->axis);
	proj_point = vec3f_add(cylinder->base, vec3f_scale(cylinder->axis,
				height_pos));
	return (vec3f_normalize(vec3f_sub(point, proj_point)));
}

void	build_geo_cylinder(const t_scene *scene,
			const t_ray *ray,
			const t_cylinder *cylinder,
			t_hit *hit)
{
	const t_surface	*surface;
	t_vec3f				normal;
	t_bump_apply		bump;

	surface = &scene->surfaces[hit->surface_id];
	normal = cylinder_normal_raw(cylinder, hit->point);
	hit->geo_normal = normal;
	if (surface_needs_uv(surface))
	{
		cylinder_build_uv(cylinder, hit);
		surface_apply_uv(surface, hit);
	}
	if (surface_needs_bump(surface))
	{
		bump.scene = scene;
		bump.surface = surface;
		cylinder_build_tbn(cylinder, normal, &bump.tangent, &bump.bitangent);
		bump.u = hit->u;
		bump.v = hit->v;
		bump_apply_normal(&bump, &normal);
	}
	hit->normal = normal;
	hit_faceforward(ray, hit);
}
