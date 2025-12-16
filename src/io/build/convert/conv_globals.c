/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conv_globals.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 16:24:42 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/16 16:24:43 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scene.h"
#include "convert.h"
#include "io.h"
#include "errors.h"
#include <stdlib.h>
#include <string.h>
#include "color.h"

void	conv_globals(const t_scene_parsed *parsed, t_scene *scene)
{
	scene->resolution_width = parsed->globals.res_width;
	scene->resolution_height = parsed->globals.res_height;
	rgb8_to_linear_vec((int [3]){
		(int)(parsed->globals.color[0]),
		(int)(parsed->globals.color[1]),
		(int)(parsed->globals.color[2])},
		&scene->ambient_color);
	scene->ambient_color[0] *= parsed->globals.brightness;
	scene->ambient_color[1] *= parsed->globals.brightness;
	scene->ambient_color[2] *= parsed->globals.brightness;
	scene->skybox.texture_id = parsed->skybox.texture_id;
	scene->skybox.mode = (int)parsed->skybox.mode;
	scene->skybox.intensity[0] = parsed->skybox.intensity[0];
	scene->skybox.intensity[1] = parsed->skybox.intensity[1];
	scene->skybox.intensity[2] = parsed->skybox.intensity[2];
}
