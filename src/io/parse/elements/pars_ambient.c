/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_ambient.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 16:43:50 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 06:41:09 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include "errors.h"
#include "../parsing_internal.h"

int	pars_ambient(t_pars_state *st, t_scene_parsed *scene)
{
	t_tok	token;
	float	brightness;
	int		rgb[3];

	if (scene->presence_mask & PRESENCE_AMBIENT)
		return (perr(ERR_PARS, PERR_AMBIENT_REDEF));
	if (!pars_next_tok(st, &token)
		|| scan_float(token, &brightness)
		|| brightness < 0.0f || brightness > 1.0f)
		return (ERR_PARS);
	if (!pars_next_tok(st, &token)
		|| scan_color(token, rgb))
		return (ERR_PARS);
	scene->globals.brightness = brightness;
	scene->globals.color[0] = (float)rgb[0];
	scene->globals.color[1] = (float)rgb[1];
	scene->globals.color[2] = (float)rgb[2];
	scene->presence_mask |= PRESENCE_AMBIENT;
	return (SUCCESS);
}
