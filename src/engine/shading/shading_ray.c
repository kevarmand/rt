/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shading_ray.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 21:10:33 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/28 19:05:49 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include <math.h>

static void	reset_hit(t_hit *hit)
{
	hit->kind = HIT_NONE;
	hit->t = INFINITY;
	hit->point = (t_vec3f){0.0f, 0.0f, 0.0f, 0.0f};
	hit->normal = (t_vec3f){0.0f, 0.0f, 0.0f, 0.0f};
	hit->primitive_id = -1;
}

int shading_ray(const t_scene *scene, const t_ray *ray, t_vec3f *color_out)
{
	t_hit	hit;

	reset_hit(&hit);
	if (scene_hit(scene, ray, &hit))
	{
		hit_build_geometry(scene, ray, &hit);
		shade_hit(scene, &hit, color_out);//peut etre regarder lerreur si un jour y en a une 
	}
	else
	{
		//background color =bleu ciel
		(*color_out)[0] = 0.53f;
		(*color_out)[1] = 0.81f;
		(*color_out)[2] = 0.92f;

	}
	return (0);
}
