/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_plane.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 16:44:27 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/18 21:48:47 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include "errors.h"
#include "../parsing_internal.h"

int	pars_plane(t_pars_state *st, t_scene_parsed *scene)
{
	t_parsed_element	parsed;
	t_tok				token;
	int					status;

	init_parsed_element(&parsed, scene);
	parsed.type = ELEM_PLANE;
	if (!pars_next_tok(st, &token) || scan_point(token,
			parsed.u_data.plane.origin))
		return (ERR_PARS);
	if (!pars_next_tok(st, &token) || scan_vec3(token,
			parsed.u_data.plane.normal))
		return (ERR_PARS);
	if (!pars_next_tok(st, &token) || scan_color(token, parsed.rgb))
		return (ERR_PARS);
	status = pars_options(st, &parsed.options, &scene->textures);
	if (status != SUCCESS)
		return (ERR_PARS);
	status = pars_register_element(scene, &parsed, ELEM_ROLE_NORMAL);
	return (status);
}
