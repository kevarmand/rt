/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 16:49:56 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/16 16:49:57 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include "type.h"
#include "errors.h"
#include "parsing_internal.h"

int	pars_line(const char *line, t_scene_parsed *scene, t_pars_state *st)
{
	t_tok	tag;

	st->line = line;
	st->pos = 0;
	if (!pars_next_tok(st, &tag))
		return (SUCCESS);
	return (pars_dispatch_tok(tag, st, scene));
}
