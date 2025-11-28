/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inter_planes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 21:35:35 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/28 15:03:25 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include "scene.h"
#include <math.h>
#include <stdio.h>

int inter_plane(const t_plane *plane, const t_ray *ray, float *out_distance)
{
	float	denom;
	float	numerator;
	float	t;

	denom = vec3f_dot(plane->normal, ray->dir);
	if (fabsf(denom) < 1e-6f)
		return (0);
	numerator = - (vec3f_dot(plane->normal, ray->origin) + plane->d);
	t = numerator / denom;
	if (t <= 0.0f)
		return (0);
	*out_distance = t;
	// printf("plane hit at t=%f\n", t);
	return (1);
}
