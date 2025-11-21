/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inter_planes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 21:35:35 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/21 21:51:24 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include "scene.h"
#include <math.h>


/***
 * typedef struct s_plane
{
	t_vec3f normal; // unitaire
	float   d;      // plan: dot(normal, X) + d = 0
}	t_plane;
 */
static int inter_plane(t_primitive *plane, const t_ray *ray, t_hit *out_hit,int index)
{
	//Implémentation de l'intersection rayon-plan infini
	float denom;
	float t;
	denom = vec3f_dot(plane->pl.normal, ray->dir);
	if (fabs(denom) > 1e-6) //éviter la division par zéro
	{
		t = -(vec3f_dot(plane->pl.normal, ray->origin) + plane->pl.d) / denom;
		if (t >= 0.0f && t < out_hit->t)
		{
			out_hit->kind = HIT_PLANE;
			out_hit->t = t;
			out_hit->point = vec3f_add(ray->origin, vec3f_scale(ray->dir, t));
			out_hit->normal = plane->pl.normal;
			out_hit->primitive_id = index;
		}
		return (1);
	}
	return (0);
}

int inter_planes(t_scene *scene, const t_ray *ray, t_hit *out_hit)
{
	int status;
	int i;

	while (i < scene->plane_count)
	{
		status |= inter_plane(&scene->planes[i], ray, out_hit, i);
		i++;
	}
	return (status);
}