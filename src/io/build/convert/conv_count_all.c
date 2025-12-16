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

static int	list_count(t_list *lst)
{
	int	count;

	count = 0;
	while (lst)
	{
		count++;
		lst = lst->next;
	}
	return (count);
}

static void	count_objects_planes(t_list *lst, int *obj_out, int *plane_out)
{
	t_parsed_element	*elem;
	int					obj_count;
	int					plane_count;

	obj_count = 0;
	plane_count = 0;
	while (lst)
	{
		elem = (t_parsed_element *)lst->content;
		if (elem->type == ELEM_PLANE)
			plane_count++;
		else
			obj_count++;
		lst = lst->next;
	}
	*obj_out = obj_count;
	*plane_out = plane_count;
}

void	conv_count_all(const t_scene_parsed *parsed, t_conv_ctx *cx)
{
	count_objects_planes(parsed->objects, &cx->object_count, &cx->plane_count);
	cx->light_count = list_count(parsed->lights);
	cx->camera_count = list_count(parsed->cameras);
}
