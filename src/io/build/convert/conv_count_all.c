/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conv_count_all.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 16:31:56 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/16 16:32:16 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "convert.h"
#include "io.h"
#include "errors.h"
#include <stdlib.h>

static void	count_objects_planes(const t_vector *objects,
		int *obj_out, int *plane_out)
{
	const t_parsed_element	*elements;
	size_t					i;
	int						plane_count;

	elements = (const t_parsed_element *)objects->data;
	i = 0;
	plane_count = 0;
	while (i < objects->count)
	{
		if (elements[i].type == ELEM_PLANE)
			plane_count++;
		i++;
	}
	*plane_out = plane_count;
	*obj_out = (int)objects->count - plane_count;
}

void	conv_count_all(const t_scene_parsed *parsed, t_conv_ctx *cx)
{
	count_objects_planes(&parsed->objects, &cx->object_count, &cx->plane_count);
	cx->light_count = (int)parsed->lights.count;
	cx->camera_count = (int)parsed->cameras.count;
}
