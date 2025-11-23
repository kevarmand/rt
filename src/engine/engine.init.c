/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 13:04:56 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/23 15:32:46 by kearmand         ###   ########.fr       */
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

int	engine_init(t_engine *engine, const t_scene *scene)
{
	int	status;
	
	engine_set_defaults(engine, scene);
	status = init_cam_views(engine, scene);
	if (status != SUCCESS)
		return (status);

	//petite place pour le BVH ICI

	status = render_init(&engine->render, engine);
	if (status != SUCCESS)
		return (status);

	return (SUCCESS);
}