/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   geometry_common.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 19:17:55 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/19 17:42:29 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../geometry.h"

int	surface_needs_uv(const t_surface *surface)
{
	return (surface->checker_mode != 0
		|| surface->texture_albedo_id != SCENE_ID_NONE
		|| surface->bump_normal_id != SCENE_ID_NONE);
}

int	surface_needs_bump(const t_surface *surface)
{
	return (surface->bump_normal_id != SCENE_ID_NONE);
}

void	hit_faceforward(const t_ray *ray, t_hit *hit)
{
	if (vec3f_dot(hit->normal, ray->dir) > 0.0f)
	{
		hit->normal = vec3f_scale(hit->normal, -1.0f);
		hit->geo_normal = vec3f_scale(hit->geo_normal, -1.0f);
	}
}
