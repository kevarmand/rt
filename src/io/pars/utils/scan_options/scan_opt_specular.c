/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan_opt_specular.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 16:09:49 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/13 16:09:50 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include "type.h"
#include "errors.h"
#include "../../parsing_internal.h"

int	scan_opt_specular(t_tok tok, t_element_options *opts,
	t_texture_parsed *texture)
{
	float	value;

	(void)texture;
	if (scan_float(tok, &value))
		return (ERR_PARSE_FLOAT);
	if (value < 0.0f || value > 1.0f)
		return (ERR_PARSE_FLOAT);
	opts->specular_weight = value;
	return (0);
}
