/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan_opt_ambient.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 16:08:08 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 06:32:32 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include "errors.h"
#include "../../parsing_internal.h"

int	scan_opt_ambient(t_tok tok, t_element_options *opts,
	t_texture_parsed *texture)
{
	float	value;

	(void)texture;
	if (scan_float(tok, &value))
		return (ERR_PARS);
	if (value < 0.0f || value > 1.0f)
		return (ERR_PARS);
	opts->ambient_occlusion = value;
	return (0);
}
