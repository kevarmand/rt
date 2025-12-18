/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_option.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 16:48:43 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/16 16:48:55 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include "type.h"
#include "errors.h"
#include "../parsing_internal.h"

/***
 * Parse options for an element from the token stream.
 * @param st      The current parsing state. (no_null)
 * @param options Pointer to the element options structure to populate. (no_null)
 * @return Returns 0 on success, or a non-zero error code on failure.
 * @note The function reads tokens until no more valid options are found.
 * Each option is expected to be in the format "key=value".
 */
int	pars_options(t_pars_state *st, t_element_options *options,
	t_texture_parsed *tex_parsed)
{
	t_tok	token;
	int		ret;

	while (pars_next_tok(st, &token))
	{
		ret = scan_option(token, options, tex_parsed);
		if (ret != SUCCESS)
			return (ret);
	}
	return (SUCCESS);
}
