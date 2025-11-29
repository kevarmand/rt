/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inter_sphere.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 14:43:12 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/29 11:31:29 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include <math.h>
#include "scene.h"
#include <stdio.h>
#include "vector.h"
int	inter_sphere(const t_sphere *sphere,
			const t_ray *ray, float *out_distance)
{
	t_vec3f	offset;
	float	coef_a;
	float	coef_b;
	float	coef_c;
	double	solution_t;

	offset = vec3f_sub(ray->origin, sphere->center);
	coef_a = vec3f_dot(ray->dir, ray->dir);
	coef_b = 2.0 * vec3f_dot(offset, ray->dir);
	coef_c = vec3f_dot(offset, offset)
		- (sphere->radius * sphere->radius);
	solution_t = equa_second(coef_a, coef_b, coef_c);

	//on afficher t_ray
	if (solution_t <= 0.0)
		return (0);
	*out_distance = (float)solution_t;
	return (1);
}