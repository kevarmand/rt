/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_register_element.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 16:47:09 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 06:40:24 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include "errors.h"
#include "libft.h"
#include "../parsing_internal.h"
#include "scene.h"
#include <limits.h>

static int	push_copy_to_vector(t_vector *elements,
				const t_parsed_element *element)
{
	if (elements->count >= INT_MAX)
		return (perr(ERR_PARS, "Too many scene elements."));
	if (vector_push_back(elements, element) != SUCCESS)
		return (perr(ERR_MALLOC, PERR_M_VEC_PUSH));
	return (SUCCESS);
}

static int	is_object_type(t_element_type type)
{
	return (type == ELEM_SPHERE
		|| type == ELEM_PLANE
		|| type == ELEM_CYLINDER
		|| type == ELEM_TRIANGLE
		|| type == ELEM_TORUS);
}

int	pars_register_element(t_scene_parsed *scene,
			const t_parsed_element *element,
			t_elem_role role)
{
	if (is_object_type(element->type))
	{
		scene->presence_mask |= PRESENCE_OBJECT;
		return (push_copy_to_vector(&scene->objects, element));
	}
	if (element->type == ELEM_CAMERA)
	{
		if (role == ELEM_ROLE_PRIMARY && (scene->presence_mask & PRESENCE_CAM))
			return (perr(ERR_PARS, PERR_CAM_REDEF));
		if (role == ELEM_ROLE_PRIMARY)
			scene->presence_mask |= PRESENCE_CAM;
		return (push_copy_to_vector(&scene->cameras, element));
	}
	if (element->type == ELEM_LIGHT)
	{
		if (role == ELEM_ROLE_PRIMARY
			&& (scene->presence_mask & PRESENCE_LIGHT))
			return (perr(ERR_PARS, PERR_LIGHT_REDEF));
		if (role == ELEM_ROLE_PRIMARY)
			scene->presence_mask |= PRESENCE_LIGHT;
		return (push_copy_to_vector(&scene->lights, element));
	}
	return (perr(ERR_PARS, "Unknown element type."));
}
