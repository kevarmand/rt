/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan_option.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 16:36:14 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/18 21:32:21 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include "errors.h"
#include "../../parsing_internal.h"

typedef int					(*t_opt_handler)(t_tok, t_element_options *,
							t_texture_parsed *);
typedef struct s_opt_entry {
	const char		*key;
	t_opt_handler	fn;
}							t_opt_entry;

static const t_opt_entry	g_opt_tab[] = {{"ior", scan_opt_ior},
{"refraction", scan_opt_refraction},
{"reflection", scan_opt_reflection},
{"shininess", scan_opt_shininess},
{"diffuse", scan_opt_diffuse},
{"specular", scan_opt_specular},
{"ambient", scan_opt_ambient},
{"ior", scan_opt_ior},
{"bump", scan_opt_bump},
{"texture", scan_opt_texture},
{"uv", scan_opt_uv},
{"checkerboard", scan_opt_checkerboard},
{"bscale", scan_opt_bscale},
{NULL, NULL}
};

static int	split_key_value(t_tok tok, t_tok *key_tok, t_tok *val_tok)
{
	int						index;

	index = 0;
	while (index < tok.len && tok.start[index] != '=')
		index++;
	if (index == 0 || index >= tok.len - 1)
		return (ERR_PARS);
	key_tok->start = tok.start;
	key_tok->len = index;
	val_tok->start = tok.start + index + 1;
	val_tok->len = tok.len - index - 1;
	return (SUCCESS);
}

int	scan_option(t_tok tok, t_element_options *opts, t_texture_parsed *texture)
{
	t_tok	key_tok;
	t_tok	val_tok;
	int		table_index;

	if (split_key_value(tok, &key_tok, &val_tok))
		return (ERR_PARS);
	table_index = 0;
	while (g_opt_tab[table_index].key)
	{
		if (pars_tok_eq(key_tok, g_opt_tab[table_index].key))
			return (g_opt_tab[table_index].fn(val_tok, opts, texture));
		table_index++;
	}
	return (ERR_PARS);
}
