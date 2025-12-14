/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsed_scene_lifetime.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 17:33:50 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/14 16:33:11 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include "libft.h"
#include "errors.h"
#include <stdio.h>
#include "pars/parsing_internal.h"

void	free_parsed_scene(t_scene_parsed *parsed)
{
	ft_lstclear(&parsed->objects, free);
	ft_lstclear(&parsed->cameras, free);
	ft_lstclear(&parsed->lights, free);
	parsed->objects = NULL;
	parsed->cameras = NULL;
	parsed->lights = NULL;
}
void	init_parsed_scene(t_scene_parsed *parsed)
{
	int	status;

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
	parsed->textures.h_texture = hashmap_create(16);
	if (status != SUCCESS)
		return ;
	parsed->skybox.texture_id = -1;
	parsed->skybox.mode = SKYBOX_SPHERE;
	parsed->skybox.intensity[0] = 1.0f;
	parsed->skybox.intensity[1] = 1.0f;
	parsed->skybox.intensity[2] = 1.0f;
	printf("Hashmap for textures created successfully\n");
	parsed->textures.index = 0;
	init_element_options(&parsed->default_options);
}

void	init_element_options(t_element_options *options)
{
	options->ior = 1.0f;
	options->reflection = 0.0f;
	options->refraction = 0.0f;
	options->shininess = 32.0f;
	options->diffuse_weight = 1.0f;
	options->specular_weight = 0.3f;
	options->ambient_occlusion = 0.0f;
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
}

void	init_parsed_element(t_parsed_element *elem, t_scene_parsed *scene)
{
	elem->type = ELEM_NONE;
	elem->options = scene->default_options;
}

