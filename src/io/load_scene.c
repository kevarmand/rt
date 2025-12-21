/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_scene.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 16:15:00 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 08:43:05 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include <errors.h>
#include "scene.h"
#include "parse/parsing_internal.h"
#include "logs.h"

static int	check_minimum_requirements(t_scene_parsed *parsed)
{
	if (!(parsed->presence_mask & PRESENCE_AMBIENT))
		return (perr(ERR_INTERNAL,
				"Scene must contain ambient lighting."));
	if (!(parsed->presence_mask & PRESENCE_CAM))
		return (perr(ERR_INTERNAL,
				"Scene must contain at least one camera."));
	if (!(parsed->presence_mask & PRESENCE_LIGHT))
		return (perr(ERR_INTERNAL,
				"Scene must contain at least one light source."));
	return (SUCCESS);
}

int	load_scene(const char *path, t_scene *out_scene)
{
	t_scene_parsed	parsed;
	int				status;

	log_step(LOGSTEP_PARSE_INIT_SCENE, 0);
	init_parsed_scene(&parsed);
	status = pars_scene(path, &parsed);
	if (status == SUCCESS)
		status = check_minimum_requirements(&parsed);
	log_step(LOGSTEP_PARSE_READ_FILE, status);
	if (status == SUCCESS)
		status = build_scene(out_scene, &parsed);
	log_step(LOGSTEP_PARSE_BUILD_CONVERT, status);
	free_parsed_scene(&parsed);
	log_step(LOGSTEP_PARSE_FREE_PARSED, 0);
	if (status == SUCCESS)
		log_info_parsed(out_scene, path);
	return (status);
}
