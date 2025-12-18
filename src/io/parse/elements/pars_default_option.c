/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_default_option.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 18:38:57 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/18 21:31:10 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include "errors.h"
#include "../parsing_internal.h"

int	pars_default_option(t_pars_state *st, t_scene_parsed *scene)
{
	int	saved_pos;
	int	status;

	saved_pos = st->pos;
	status = pars_options(st, &scene->default_options, &scene->textures);
	if (status != SUCCESS)
		return (ERR_PARS);
	if (st->pos == saved_pos)
		init_element_options(&scene->default_options);
	return (SUCCESS);
}
