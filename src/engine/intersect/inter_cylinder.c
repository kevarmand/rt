/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inter_cylinder.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 14:43:38 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/10 04:07:29 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include <math.h>
#include "scene.h"
#include <math.h>

int	solve_quadratic(double coeff_a,
			double coeff_b,
			double coeff_c,
			double roots[2])
{
	double	discriminant;
	double	sqrt_discriminant;
	double	temp_q;

	if (coeff_a == 0.0)
		return (0);
	discriminant = coeff_b * coeff_b - 4.0 * coeff_a * coeff_c;
	if (discriminant < 0.0)
		return (0);
	sqrt_discriminant = sqrt(discriminant);
	if (coeff_b >= 0.0)
		temp_q = -0.5 * (coeff_b + sqrt_discriminant);
	else
		temp_q = -0.5 * (coeff_b - sqrt_discriminant);
	roots[0] = temp_q / coeff_a;
	if (temp_q == 0.0)
		roots[1] = roots[0];
	else
		roots[1] = coeff_c / temp_q;
	if (roots[0] > roots[1])
	{
		temp_q = roots[0];
		roots[0] = roots[1];
		roots[1] = temp_q;
	}
	return (1);
}

int	inter_cylinder(const t_cylinder *cylinder,
			const t_ray *ray,
			float *out_distance)
{
	t_vec3f	axis;
	t_vec3f	origin_to_base;
	t_vec3f	perp_dir;
	t_vec3f	perp_offset;
	float	dot_dir_axis;
	float	dot_origin_axis;
	double	quad_a;
	double	quad_b;
	double	quad_c;
	double	roots[2];
	double	best_t;
	float	height_position;

	axis = cylinder->axis;
	origin_to_base = vec3f_sub(ray->origin, cylinder->base);
	dot_dir_axis = vec3f_dot(ray->dir, axis);
	dot_origin_axis = vec3f_dot(origin_to_base, axis);
	perp_dir = vec3f_sub(ray->dir,
			vec3f_scale(axis, dot_dir_axis));
	perp_offset = vec3f_sub(origin_to_base,
			vec3f_scale(axis, dot_origin_axis));
	quad_a = vec3f_dot(perp_dir, perp_dir);
	quad_b = 2.0 * vec3f_dot(perp_dir, perp_offset);
	quad_c = vec3f_dot(perp_offset, perp_offset)
		 - cylinder->r_squared;
	if (!solve_quadratic(quad_a, quad_b, quad_c, roots))
		return (0);
	best_t = -1.0;
	if (roots[0] > 0.0)
	{
		height_position = dot_origin_axis
			+ dot_dir_axis * (float)roots[0];
		if (height_position >= 0.0f
			&& height_position <= cylinder->height)
			best_t = roots[0];
	}
	if (roots[1] > 0.0)
	{
		height_position = dot_origin_axis
			+ dot_dir_axis * (float)roots[1];
		if (height_position >= 0.0f
			&& height_position <= cylinder->height
			&& (best_t < 0.0 || roots[1] < best_t))
			best_t = roots[1];
	}
	if (best_t <= 0.0)
		return (0);
	*out_distance = (float)best_t;
	return (1);
}
