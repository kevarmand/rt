/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conv_cameras.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 16:31:33 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/18 21:48:47 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scene.h"
#include "convert.h"
#include "io.h"
#include "libft.h"
#include "errors.h"
#include <stdlib.h>
#include <string.h>
#include "vector.h"

static void	cam_from_parsed(const t_parsed_element *src, t_camera *dst)
{
	vec3f_load3(&dst->origin, src->u_data.camera.position);
	vec3f_load3(&dst->forward, src->u_data.camera.orientation);
	dst->fov_deg = (float)src->u_data.camera.fov;
}

int	conv_cameras(t_scene_parsed *parsed, t_conv_ctx *cx)
{
	t_list		*list_node;
	t_camera	camera_tmp;

	if (vector_reserve(&cx->cam_v, cx->camera_count) != SUCCESS)
		return (ERR_MALLOC);
	list_node = parsed->cameras;
	while (list_node)
	{
		cam_from_parsed(list_node->content, &camera_tmp);
		if (vector_push_back(&cx->cam_v, &camera_tmp) != SUCCESS)
			return (ERR_MALLOC);
		list_node = list_node->next;
	}
	return (SUCCESS);
}
