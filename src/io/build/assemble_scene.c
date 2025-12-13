/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assemble_scene.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 10:24:15 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/13 16:39:28 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include "scene.h"
#include "errors.h"
#include "convert/convert.h"

static int	assemble_array(t_vector *vec, size_t elem_size,
			void **out_array, int *out_count)
{
	int		count;
	void	*src_ptr;

	count = vector_size(vec);
	*out_count = count;
	*out_array = NULL;
	if (count <= 0)
		return (SUCCESS);
	*out_array = malloc(elem_size * count);
	if (!*out_array)
		return (ERR_MALLOC);
	src_ptr = vector_data(vec);
	ft_memcpy(*out_array, src_ptr, elem_size * count);
	return (SUCCESS);
}

int	assemble_scene(t_conv_ctx *cx, t_scene *scene)
{
	if (assemble_array(&cx->obj_v, sizeof(t_primitive),
			(void **)&scene->primitives, &scene->primitive_count) != SUCCESS)
		return (ERR_MALLOC);
	if (assemble_array(&cx->plane_v, sizeof(t_primitive),
			(void **)&scene->planes, &scene->plane_count) != SUCCESS)
		return (ERR_MALLOC);
	if (assemble_array(&cx->cam_v, sizeof(t_camera),
			(void **)&scene->cameras, &scene->camera_count) != SUCCESS)
		return (ERR_MALLOC);
	if (assemble_array(&cx->light_v, sizeof(t_light),
			(void **)&scene->lights, &scene->light_count) != SUCCESS)
		return (ERR_MALLOC);
	if (assemble_array(&cx->surf_v, sizeof(t_surface),
			(void **)&scene->surfaces, &scene->surface_count) != SUCCESS)
		return (ERR_MALLOC);
	if (assemble_array(&cx->mat_v, sizeof(t_material),
			(void **)&scene->materials, &scene->material_count) != SUCCESS)
		return (ERR_MALLOC);
	return (SUCCESS);
}
