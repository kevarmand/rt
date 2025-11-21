/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conv_ctx_lifetime.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 19:30:25 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/21 16:18:17 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "convert/convert.h"
#include "io.h"
#include "errors.h"
#include "libft.h"
#include "scene.h"

void	init_ctx(t_conv_ctx *cx)
{
	ft_memset(cx, 0, sizeof(t_conv_ctx));
	cx->tex_m = hashmap_create(256);
	cx->mat_m = hashmap_create(256);
	vector_init(&cx->tex_v, 0, sizeof(t_texture));
	vector_init(&cx->mat_v, 0, sizeof(t_material));
	vector_init(&cx->surf_v, 0, sizeof(t_surface));
	vector_init(&cx->cam_v, 0, sizeof(t_camera));
	vector_init(&cx->light_v, 0, sizeof(t_light));
	vector_init(&cx->plane_v, 0, sizeof(t_primitive));
	vector_init(&cx->obj_v, 0, sizeof(t_primitive));
}


static void	destroy_texture(t_vector tex_vector)
{
	int			i;
	int			count;
	t_texture	*tex_data;
	char		*path;
	
	i = 0;
	count = vector_size(&tex_vector);
	while (i < count)
	{
		tex_data = (t_texture *)vector_at(&tex_vector, i);
		path = tex_data->path;
		if (path)
			free(path);
		i++;
	}
}

void	free_ctx(t_conv_ctx *cx)
{
	destroy_texture(cx->tex_v);
	vector_destroy(&cx->tex_v);
	hashmap_destroy(cx->tex_m, NULL);
	vector_destroy(&cx->mat_v);
	hashmap_destroy(cx->mat_m, NULL);
	vector_destroy(&cx->surf_v);
	vector_destroy(&cx->cam_v);
	vector_destroy(&cx->light_v);
	vector_destroy(&cx->plane_v);
	vector_destroy(&cx->obj_v);
}