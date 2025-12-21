/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_camera.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 16:43:59 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 04:25:49 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include "errors.h"
#include "vector.h"
#include "../parsing_internal.h"

int	pars_camera(t_pars_state *st, t_scene_parsed *scene, t_elem_role role)
{
	t_parsed_element	parsed_element;
	t_tok				token;
	float				fov_as_float;
	int					status;

	init_parsed_element(&parsed_element, scene);
	parsed_element.type = ELEM_CAMERA;
	if (!pars_next_tok(st, &token) || scan_point(token,
			parsed_element.u_data.camera.position))
		return (ERR_PARS);
	if (!pars_next_tok(st, &token) || scan_vec3(token,
			parsed_element.u_data.camera.orientation))
		return (ERR_PARS);
	if (!pars_next_tok(st, &token) || scan_float(token,
			&fov_as_float))
		return (ERR_PARS);
	parsed_element.u_data.camera.fov = (int)fov_as_float;
	if (parsed_element.u_data.camera.fov < 1
		|| parsed_element.u_data.camera.fov > 179)
		return (ERR_PARS);
	status = pars_options(st, &parsed_element.options, &scene->textures);
	if (status != 0)
		return (ERR_PARS);
	return (pars_register_element(scene, &parsed_element, role));
}
