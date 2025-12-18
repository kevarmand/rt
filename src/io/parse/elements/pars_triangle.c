/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_triangle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 16:46:49 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/18 21:48:47 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include "errors.h"
#include "../parsing_internal.h"

int	pars_triangle(t_pars_state *st, t_scene_parsed *scene)
{
	t_parsed_element	parsed_element;
	t_tok				token;
	int					status;

	init_parsed_element(&parsed_element, scene);
	parsed_element.type = ELEM_TRIANGLE;
	if (!pars_next_tok(st, &token) || scan_point(token,
			parsed_element.u_data.triangle.vertex1))
		return (ERR_PARS);
	if (!pars_next_tok(st, &token) || scan_point(token,
			parsed_element.u_data.triangle.vertex2))
		return (ERR_PARS);
	if (!pars_next_tok(st, &token) || scan_point(token,
			parsed_element.u_data.triangle.vertex3))
		return (ERR_PARS);
	if (!pars_next_tok(st, &token) || scan_color(token,
			parsed_element.rgb))
		return (ERR_PARS);
	status = pars_options(st, &parsed_element.options, &scene->textures);
	if (status != 0)
		return (ERR_PARS);
	return (pars_register_element(scene, &parsed_element, ELEM_ROLE_NORMAL));
}
