/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inter_prim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 21:52:31 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/21 22:32:34 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include <math.h>
#include "scene.h"

double	equa_second(double a, double b, double c);

static int	inter_sphere(const t_primitive *primitive,
			const t_ray *ray, t_hit *out_hit, int index)
{
	t_vec3f	offset;
	double	coef_a;
	double	coef_b;
	double	coef_c;
	double	solution_t;

	offset = vec3f_sub(ray->origin, primitive->sp.center);
	coef_a = vec3f_dot(ray->dir, ray->dir);
	coef_b = 2.0 * vec3f_dot(offset, ray->dir);
	coef_c = vec3f_dot(offset, offset)
		- primitive->sp.radius * primitive->sp.radius;
	solution_t = equa_second(coef_a, coef_b, coef_c);
	if (solution_t <= 0.0 || solution_t >= out_hit->t)
		return (0);
	out_hit->kind = HIT_PRIMITIVE;
	out_hit->t = (float)solution_t;
	out_hit->point = vec3f_add(ray->origin,
			vec3f_scale(ray->dir, (float)solution_t));
	out_hit->normal = vec3f_normalize(vec3f_sub(out_hit->point,
				primitive->sp.center));
	out_hit->primitive_id = index;
	return (1);
}

static int	inter_cylinder(const t_primitive *primitive,
			const t_ray *ray, t_hit *hit, int index)
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

	axis = primitive->cy.dir;
	origin_to_base = vec3f_sub(ray->origin, primitive->cy.
	dv = vec3f_dot(ray->dir, axis);
	ocv = vec3f_dot(origin_to_base, axis);
	perp_dir = vec3f_sub(ray->dir, vec3f_scale(axis, dv));
	perp_offset = vec3f_sub(origin_to_base,
			vec3f_scale(axis, ocv));
	a = vec3f_dot(perp_dir, perp_dir);
	b = 2.0 * vec3f_dot(perp_dir, perp_offset);
	c = vec3f_dot(perp_offset, perp_offset)
		- primitive->cy.radius * primitive->cy.radius;
	t = equa_second(a, b, c);
	if (t <= 0.0 || t >= hit->distance)
		return (0);
	y = ocv + dv * (float)t;
	if (y < 0.0f || y > primitive->cy.height)
		return (0);
	hit->kind = HIT_PRIMITIVE;
	hit->distance = (float)t;
	hit->point = vec3f_add(ray->origin,
			vec3f_scale(ray->dir, hit->distance));
	hit->normal = vec3f_normalize(vec3f_sub(hit->point,
				vec3f_add(primitive->cy.base,
					vec3f_scale(axis, y))));
	hit->primitive_id = index;
	return (1);
}

static int	inter_torus(const t_primitive *primitive,
			const t_ray *ray, t_hit *out_hit, int index)
{
	(void)primitive;
	(void)ray;
	(void)out_hit;
	(void)index;
	//Implémentation de l'intersection rayon-tore (à compléter)
	write(2, "inter_torus: Not implemented yet.\n", 33);
	return (0);
}

static int	inter_triangle(const t_primitive *primitive,
			const t_ray *ray, t_hit *out_hit, int index)
{
	const t_triangle	*triangle;
	t_vec3f				cross_dir_edge2;
	t_vec3f				origin_to_v0;
	t_vec3f				cross_s_edge1;
	float				determinant;
	float				inverse_det;
	float				bary_u;
	float				bary_v;
	float				hit_distance;

	triangle = &primitive->tr;
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
	hit_distance = inverse_det * vec3f_dot(triangle->edge2, cross_s_edge1);
	if (hit_distance <= 0.0f || hit_distance >= out_hit->t)
		return (0);
	out_hit->kind = HIT_PRIMITIVE;
	out_hit->t = hit_distance;
	out_hit->point = vec3f_add(ray->origin,
			vec3f_scale(ray->dir, hit_distance));
	out_hit->normal = vec3f_normalize(
			vec3f_cross(triangle->edge1, triangle->edge2));
	out_hit->primitive_id = index;
	return (1);
}


	


int	inter_primitives(t_primitive *prim, const t_ray *ray, t_hit *out_hit)
{
	int status;
	int i;

	status = 0;
	i = 0;
	while (i < 1)
	{
		if (prim->type == PRIM_SPHERE)
		{
			status |= inter_sphere(prim, ray, out_hit, i);
		}
		else if (prim->type == PRIM_CYLINDER)
		{
			status |= inter_cylinder(prim, ray, out_hit, i);
		}
		else if (prim->type == PRIM_TORUS)
		{
			status |= inter_torus(prim, ray, out_hit, i);
		}
		else if (prim->type == PRIM_TRIANGLE)
		{
			status |= inter_triangle(prim, ray, out_hit, i);
		}
		i++;
	}
	return (status);
}