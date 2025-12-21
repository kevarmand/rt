/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_scene.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 16:15:00 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 02:22:36 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include <stdio.h>
#include <errors.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "scene.h"
#include "parse/parsing_internal.h"
#include "logs.h"

void	print_scene2(const t_scene_parsed *scene);

int	load_scene(const char *path, t_scene *out_scene)
{
	t_scene_parsed	parsed;
	int				status;

	log_step(LOGSTEP_PARSE_INIT_SCENE, 0);
	init_parsed_scene(&parsed);
	status = pars_scene(path, &parsed);
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
