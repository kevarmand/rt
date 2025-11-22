/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inter_cylinder.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 14:43:38 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/22 17:35:47 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include <math.h>
#include "scene.h"

int	inter_cylinder(const t_cylinder *cylinder,
			const t_ray *ray, float *out_distance)
{
	t_vec3f	axis;
	t_vec3f	origin_to_base;
	t_vec3f	perp_dir;
	t_vec3f	perp_offset;
	float	dv;
	float	ocv;
	double	a;
	double	b;
	double	c;
	double	t;
	float	y;

	axis = cylinder->axis;
	origin_to_base = vec3f_sub(ray->origin, cylinder->base);
	dv = vec3f_dot(ray->dir, axis);
	ocv = vec3f_dot(origin_to_base, axis);
	perp_dir = vec3f_sub(ray->dir, vec3f_scale(axis, dv));
	perp_offset = vec3f_sub(origin_to_base, vec3f_scale(axis, ocv));
	a = vec3f_dot(perp_dir, perp_dir);
	b = 2.0 * vec3f_dot(perp_dir, perp_offset);
	c = vec3f_dot(perp_offset, perp_offset) - cylinder->radius_sq;
	t = equa_second(a, b, c);
	if (t <= 0.0)
		return (0);
	y = ocv + dv * (float)t;
	if (y < 0.0f || y > cylinder->height)
		return (0);
	*out_distance = (float)t;
	return (1);
}
