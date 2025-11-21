/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsed_scene_lifetime.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 17:33:50 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/21 15:13:13 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include "libft.h"

void free_parsed_element(void *obj)
{
	char 				*path;
	t_parsed_element	*elem;

	elem = (t_parsed_element *)obj;
	if (!elem)
		return ;
	path = elem->options.texture_path;
	if (path)
		free(path);
	elem->options.texture_path = NULL;
	path = elem->options.bumpmap_path;
	if (path)
		free(path);
	elem->options.bumpmap_path = NULL;
	free(obj);
}

void	free_parsed_scene(t_scene_parsed *parsed)
{
	ft_lstclear(&parsed->objects, free_parsed_element);
	ft_lstclear(&parsed->cameras, free_parsed_element);
	ft_lstclear(&parsed->lights, free_parsed_element);
	parsed->objects = NULL;
	parsed->cameras = NULL;
	parsed->lights = NULL;
}
void	init_parsed_scene(t_scene_parsed *parsed)
{
	parsed->objects = NULL;
	parsed->cameras = NULL;
	parsed->lights = NULL;
	parsed->globals.res_height = 600;
	parsed->globals.res_width = 800;
	parsed->globals.brightness = 0.4f;
	parsed->globals.color[0] = 0.2f;
	parsed->globals.color[1] = 0.0f;
	parsed->globals.color[2] = 0.0f;
	parsed->presence_mask = 0;
}

void	init_parsed_element(t_parsed_element *elem)
{
	elem->type = ELEM_NONE;
	elem->options.ior = 1.0f;
	elem->options.reflection = 0.0f;
	elem->options.refraction = 0.0f;
	elem->options.shininess = 0.0f;
	elem->options.diffuse_weight = 1.0f;
	elem->options.specular_weight = 1.0f;
	elem->options.ambient_occlusion = 0.0f;
	elem->options.uv[0] = 0.0f;
	elem->options.uv[1] = 0.0f;
	elem->options.uv[2] = 1.0f;
	elem->options.uv[3] = 1.0f;
	elem->options.uv[4] = 0.0f;
	elem->options.uv[5] = 0.0f;
	elem->options.uv_mod = 0;
	elem->options.texture_path = NULL;
	elem->options.bumpmap_path = NULL;
}
