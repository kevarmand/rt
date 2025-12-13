/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_torus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 10:46:44 by marvin            #+#    #+#             */
/*   Updated: 2025/12/13 10:46:44 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include "type.h"
#include "errors.h"
#include "../parsing_internal.h"

int	pars_torus(t_pars_state *st, t_scene_parsed *scene)
{
	t_parsed_element	parsed;
	t_tok				token;
	int					status;

	init_parsed_element(&parsed, scene);
	parsed.type = ELEM_TORUS;
	if (!pars_next_tok(st, &token) || scan_point(token,
			parsed.data.torus.center))
		return (ERR_PARS);
	if (!pars_next_tok(st, &token) || scan_vec3(token,
			parsed.data.torus.normal))
		return (ERR_PARS);
	if (!pars_next_tok(st, &token) || scan_float(token,
			&parsed.data.torus.major_radius))
		return (ERR_PARS);
	if (!pars_next_tok(st, &token) || scan_float(token,
			&parsed.data.torus.minor_radius))
		return (ERR_PARS);
	if (parsed.data.torus.major_radius <= 0.0f
		|| parsed.data.torus.minor_radius <= 0.0f)
		return (ERR_PARS);
	if (!pars_next_tok(st, &token) || scan_color(token, parsed.rgb))
		return (ERR_PARS);
	status = pars_options(st, &parsed.options, &scene->textures);
	if (status != SUCCESS)
		return (ERR_PARS);
	status = pars_register_element(scene, &parsed, ELEM_ROLE_NORMAL);
	return (status);
}
