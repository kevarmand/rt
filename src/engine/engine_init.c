/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 13:04:56 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/27 20:28:54 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include "errors.h"

#define RENDER_MODE_NORMAL 0 //default render mode

static void	engine_set_defaults(t_engine *engine, const t_scene *scene)
{
	engine->width = scene->resolution_width;
	engine->height = scene->resolution_height;
	engine->active_cam = 0;
	engine->render_mode = RENDER_MODE_NORMAL;
	engine->samples_per_pixel = 1;
}

int	engine_init(t_engine *engine, t_scene *scene)
{
	int	status;
	
	engine_set_defaults(engine, scene);
	init_cam_views(scene);
	
	//petite place pour le BVH ICI TODO:NOAH

	status = render_init(&engine->render, engine->width, engine->height);
	if (status != SUCCESS)
		return (status);

	return (SUCCESS);
}
