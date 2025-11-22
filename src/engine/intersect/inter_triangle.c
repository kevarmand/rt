/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inter_triangle.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 17:38:51 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/22 17:38:52 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include <math.h>
#include "scene.h"

int	inter_triangle(const t_triangle *triangle,
			const t_ray *ray, float *out_distance)
{
	t_vec3f	cross_dir_edge2;
	t_vec3f	origin_to_v0;
	t_vec3f	cross_s_edge1;
	float	determinant;
	float	inverse_det;
	float	bary_u;
	float	bary_v;

	cross_dir_edge2 = vec3f_cross(ray->dir, triangle->edge2);
	determinant = vec3f_dot(triangle->edge1, cross_dir_edge2);
	if (fabsf(determinant) < 1e-6f)
		return (0);
	inverse_det = 1.0f / determinant;
	origin_to_v0 = vec3f_sub(ray->origin, triangle->v0);
	bary_u = inverse_det * vec3f_dot(origin_to_v0, cross_dir_edge2);
	if (bary_u < 0.0f || bary_u > 1.0f)
		return (0);
	cross_s_edge1 = vec3f_cross(origin_to_v0, triangle->edge1);
	bary_v = inverse_det * vec3f_dot(ray->dir, cross_s_edge1);
	if (bary_v < 0.0f || bary_u + bary_v > 1.0f)
		return (0);
	*out_distance = inverse_det * vec3f_dot(triangle->edge2, cross_s_edge1);
	return (*out_distance > 0.0f);
}
