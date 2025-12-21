/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_primitive.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 16:34:12 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 02:15:04 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include "errors.h"
#include <stdlib.h>
#include "scene.h"

int	copy_triangle_to_primitive(const t_parsed_element *src, t_primitive *dst)
{
	dst->type = PRIM_TRIANGLE;
	dst->tr.v0 = ((t_vec3f){
			src->u_data.triangle.vertex1[0],
			src->u_data.triangle.vertex1[1],
			src->u_data.triangle.vertex1[2]});
	dst->tr.edge1 = vec3f_sub((t_vec3f){
			src->u_data.triangle.vertex2[0],
			src->u_data.triangle.vertex2[1],
			src->u_data.triangle.vertex2[2]},
			dst->tr.v0);
	dst->tr.edge2 = vec3f_sub((t_vec3f){
			src->u_data.triangle.vertex3[0],
			src->u_data.triangle.vertex3[1],
			src->u_data.triangle.vertex3[2]},
			dst->tr.v0);
	return (SUCCESS);
}

int	copy_sphere_to_primitive(const t_parsed_element *src, t_primitive *dst)
{
	dst->type = PRIM_SPHERE;
	dst->sp.center = ((t_vec3f){
			src->u_data.sphere.center[0],
			src->u_data.sphere.center[1],
			src->u_data.sphere.center[2]});
	dst->sp.radius = (src->u_data.sphere.diameter / 2.0f);
	dst->sp.r_squared = dst->sp.radius * dst->sp.radius;
	return (SUCCESS);
}

int	copy_cylinder_to_primitive(const t_parsed_element *src, t_primitive *dst)
{
	t_vec3f	center;
	t_vec3f	axis;
	float	height;
	float	radius;

	dst->type = PRIM_CYLINDER;
	center = (t_vec3f){
		src->u_data.cylinder.origin[0],
		src->u_data.cylinder.origin[1],
		src->u_data.cylinder.origin[2]};
	axis = (t_vec3f){
		src->u_data.cylinder.axis[0],
		src->u_data.cylinder.axis[1],
		src->u_data.cylinder.axis[2]};
	axis = vec3f_normalize(axis);
	height = src->u_data.cylinder.height;
	radius = src->u_data.cylinder.diameter * 0.5f;
	dst->cy.axis = axis;
	dst->cy.height = height;
	dst->cy.radius = radius;
	dst->cy.r_squared = radius * radius;
	dst->cy.base = vec3f_sub(center, vec3f_scale(axis, 0.5f * height));
	return (SUCCESS);
}

int	copy_plane_to_primitive(const t_parsed_element *src, t_primitive *dst)
{
	dst->type = PRIM_PLANE;
	dst->pl.normal = ((t_vec3f){
			src->u_data.plane.normal[0],
			src->u_data.plane.normal[1],
			src->u_data.plane.normal[2]});
	dst->pl.d = -(dst->pl.normal[0] * src->u_data.plane.origin[0]
			+ dst->pl.normal[1] * src->u_data.plane.origin[1]
			+ dst->pl.normal[2] * src->u_data.plane.origin[2]);
	return (SUCCESS);
}

int	copy_torus_to_primitive(const t_parsed_element *src, t_primitive *dst)
{
	dst->type = PRIM_TORUS;
	dst->to.center = ((t_vec3f){
			src->u_data.torus.center[0],
			src->u_data.torus.center[1],
			src->u_data.torus.center[2]});
	dst->to.normal = vec3f_normalize((t_vec3f){
			src->u_data.torus.normal[0],
			src->u_data.torus.normal[1],
			src->u_data.torus.normal[2]});
	dst->to.r0 = src->u_data.torus.major_radius;
	dst->to.r1 = src->u_data.torus.minor_radius;
	dst->to.inv_r0 = 1.0 / dst->to.r0;
	dst->to.r1_scaled = dst->to.r1 * dst->to.inv_r0;
	return (SUCCESS);
}
