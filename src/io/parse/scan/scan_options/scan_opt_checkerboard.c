/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan_opt_checkerboard.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 12:56:59 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/18 21:31:56 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include "errors.h"
#include "../../parsing_internal.h"

int	scan_opt_checkerboard(t_tok tok,
		t_element_options *opts,
		t_texture_parsed *textures)
{
	int		color[3];
	int		tex_id;

	if (pars_tok_eq(tok, "ON") || pars_tok_eq(tok, "on"))
	{
		opts->checker_mode = 1;
		opts->checker_texture_id = -1;
		return (SUCCESS);
	}
	if (scan_color(tok, color) == SUCCESS)
	{
		opts->checker_mode = 2;
		opts->checker_color[0] = color[0];
		opts->checker_color[1] = color[1];
		opts->checker_color[2] = color[2];
		opts->checker_texture_id = -1;
		return (SUCCESS);
	}
	if (intern_texture_tok(textures, tok, &tex_id) == SUCCESS)
	{
		opts->checker_mode = 3;
		opts->checker_texture_id = tex_id;
		return (SUCCESS);
	}
	return (ERR_PARS);
}
