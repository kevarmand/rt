/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conv_primitives.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 16:34:38 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 06:57:38 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "color.h"
#include "scene.h"
#include "convert.h"
#include "io.h"
#include "errors.h"
#include <stdlib.h>

// Function prototypes for copying parsed elements to primitives
int	copy_triangle_to_primitive(const t_parsed_element *src, t_primitive *dst);
int	copy_sphere_to_primitive(const t_parsed_element *src, t_primitive *dst);
int	copy_cylinder_to_primitive(const t_parsed_element *src, t_primitive *dst);
int	copy_plane_to_primitive(const t_parsed_element *src, t_primitive *dst);
int	copy_torus_to_primitive(const t_parsed_element *src, t_primitive *dst);

int	conv_option_primitive(t_primitive *prim, t_element_options *opt,
		t_conv_ctx *cx, int *color);

static int	init_primitives(t_conv_ctx *cx)
{
	if (vector_reserve(&cx->obj_v, cx->object_count) != SUCCESS)
		return (perr(ERR_MALLOC, PERR_M_VEC_OBJ));
	if (vector_reserve(&cx->plane_v, cx->plane_count) != SUCCESS)
		return (perr(ERR_MALLOC, PERR_M_VEC_PLANE));
	return (SUCCESS);
}

static void	object_from_parsed(t_parsed_element *src, t_primitive *dst)
{
	if (src->type == ELEM_TRIANGLE)
		copy_triangle_to_primitive(src, dst);
	else if (src->type == ELEM_SPHERE)
		copy_sphere_to_primitive(src, dst);
	else if (src->type == ELEM_CYLINDER)
		copy_cylinder_to_primitive(src, dst);
	else if (src->type == ELEM_PLANE)
		copy_plane_to_primitive(src, dst);
	else if (src->type == ELEM_TORUS)
		copy_torus_to_primitive(src, dst);
}

static int	push_elem_to_ctx(t_parsed_element *elem, t_conv_ctx *cx)
{
	t_primitive		prim_tmp;

	object_from_parsed(elem, &prim_tmp);
	if (conv_option_primitive(&prim_tmp, &elem->options, cx, elem->rgb))
		return (ERR_MALLOC);
	if (elem->type != ELEM_PLANE)
	{
		if (vector_push_back(&cx->obj_v, &prim_tmp) != SUCCESS)
			return (perr(ERR_MALLOC, PERR_M_VEC_PUSH));
		return (SUCCESS);
	}
	if (vector_push_back(&cx->plane_v, &prim_tmp) != SUCCESS)
		return (perr(ERR_MALLOC, PERR_M_VEC_PUSH));
	return (SUCCESS);
}

int	conv_primitives(t_scene_parsed *parsed, t_conv_ctx *cx)
{
	t_list					*node;
	t_parsed_element		*elem;

	if (init_primitives(cx) != SUCCESS)
		return (ERR_MALLOC);
	node = parsed->objects;
	while (node)
	{
		elem = node->content;
		if (push_elem_to_ctx(elem, cx) != SUCCESS)
			return (ERR_MALLOC);
		node = node->next;
	}
	return (SUCCESS);
}
