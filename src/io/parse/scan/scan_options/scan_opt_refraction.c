/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan_opt_refraction.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 16:09:16 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 06:32:08 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include "errors.h"
#include "../../parsing_internal.h"

int	scan_opt_refraction(t_tok tok, t_element_options *opts,
	t_texture_parsed *texture)
{
	float	value;

	(void)texture;
	if (scan_float(tok, &value))
		return (ERR_PARS);
	if (value < 0.0f)
		return (ERR_PARS);
	opts->refraction = value;
	return (0);
}
