/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan_color.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 16:47:45 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/18 21:32:27 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include "errors.h"
#include "../parsing_internal.h"

static int	scan_uint8_component(
	t_tok tok, int *index_pos, int *value_out, int require_comma)
{
	int	value;
	int	digits_read;

	value = 0;
	digits_read = 0;
	while (*index_pos < tok.len
		&& tok.start[*index_pos] >= '0'
		&& tok.start[*index_pos] <= '9')
	{
		value = value * 10 + (tok.start[*index_pos] - '0');
		if (value > 255)
			return (ERR_PARS);
		digits_read++;
		(*index_pos)++;
	}
	if (digits_read == 0)
		return (ERR_PARS);
	if (require_comma)
	{
		if (*index_pos >= tok.len || tok.start[*index_pos] != ',')
			return (ERR_PARS);
		(*index_pos)++;
	}
	*value_out = value;
	return (SUCCESS);
}

int	scan_color(t_tok tok, int out_rgb[3])
{
	int	index_pos;

	index_pos = 0;
	if (scan_uint8_component(tok, &index_pos, &out_rgb[0], 1))
		return (ERR_PARS);
	if (scan_uint8_component(tok, &index_pos, &out_rgb[1], 1))
		return (ERR_PARS);
	if (scan_uint8_component(tok, &index_pos, &out_rgb[2], 0))
		return (ERR_PARS);
	if (index_pos != tok.len)
		return (ERR_PARS);
	return (SUCCESS);
}
