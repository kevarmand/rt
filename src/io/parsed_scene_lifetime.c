/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsed_scene_lifetime.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 17:33:50 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 03:08:34 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include "libft.h"
#include "errors.h"
#include <stdio.h>
#include "parse/parsing_internal.h"

void	parsed_textures_destroy(t_texture_parsed *parsed);

void	free_parsed_scene(t_scene_parsed *parsed)
{
	ft_lstclear(&parsed->objects, free);
	ft_lstclear(&parsed->cameras, free);
	ft_lstclear(&parsed->lights, free);
	parsed_textures_destroy(&parsed->textures);
	parsed->objects = NULL;
	parsed->cameras = NULL;
	parsed->lights = NULL;
}

int	init_heap_element(t_scene_parsed *parsed)
{
	parsed->textures.h_texture = hashmap_create(16);
	if (parsed->textures.h_texture == NULL)
		return (ERR_MALLOC);
	parsed->textures.h_bumpmap = hashmap_create(16);
	ft_printf("\t\tHashmap for textures created successfully\n");
	if (parsed->textures.h_bumpmap == NULL)
		return (ERR_MALLOC);
	ft_printf("\t\tHashmap for bumpmaps created successfully\n");
	return (SUCCESS);
}

int	init_parsed_scene(t_scene_parsed *parsed)
{
	ft_memset(parsed, 0, sizeof(t_scene_parsed));
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
	parsed->skybox.texture_id = -1;
	parsed->skybox.mode = SKYBOX_SPHERE;
	parsed->skybox.intensity[0] = 1.0f;
	parsed->skybox.intensity[1] = 1.0f;
	parsed->skybox.intensity[2] = 1.0f;
	parsed->textures.index_t = 0;
	parsed->textures.index_b = 0;
	if (init_heap_element(parsed) != SUCCESS)
		return (ERR_MALLOC);
	init_element_options(&parsed->default_options);
	return (SUCCESS);
}

void	init_element_options(t_element_options *options)
{
	options->ior = 1.0f;
	options->reflection = 0.0f;
	options->refraction = 0.0f;
	options->shininess = 32.0f;
	options->diffuse_weight = 1.0f;
	options->specular_weight = 0.2f;
	options->ambient_occlusion = 0.2f;
	options->uv[0] = 0.0f;
	options->uv[1] = 0.0f;
	options->uv[2] = 1.0f;
	options->uv[3] = 1.0f;
	options->uv[4] = 0.0f;
	options->uv[5] = 0.0f;
	options->uv_mod = 0;
	options->texture_id = -1;
	options->bumpmap_id = -1;
	options->checker_mode = 0;
	options->checker_texture_id = -1;
	options->checker_color[0] = 0;
	options->checker_color[1] = 0;
	options->checker_color[2] = 0;
	options->bscale = 1.0f;
}

void	init_parsed_element(t_parsed_element *elem, t_scene_parsed *scene)
{
	elem->type = ELEM_NONE;
	elem->options = scene->default_options;
}
