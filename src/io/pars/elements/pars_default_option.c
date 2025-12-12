/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_default_option.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 18:38:57 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/12 19:00:59 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include "type.h"
#include "errors.h"
#include "../parsing_internal.h"

int	pars_default_option(t_pars_state *st, t_scene_parsed *scene)
{
	t_tok				token;
	int					status;

	status = pars_options(st, &scene->default_options);
	if (status != 0)
		return (ERR_PARS);
	return (0);
}
