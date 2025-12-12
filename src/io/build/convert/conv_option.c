/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conv_option.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 10:40:46 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/12 20:40:04 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include "errors.h"
#include <stdlib.h>
#include "libft.h"
#include "scene.h"
#include "convert.h"
#include <stdio.h>
int	conv_option_primitive(t_primitive *prim, t_element_options *opt,
			t_conv_ctx *cx, int *color)
{
	t_opt_ids	ids;
	t_index		mat_id;
	t_index		surf_id;

	if (intern_texture(cx, &opt->texture_path, &ids.albedo) != SUCCESS)
		return (ERR_MALLOC);
	if (intern_texture(cx, &opt->bumpmap_path, &ids.normal) != SUCCESS)
		return (ERR_MALLOC);
	if (intern_material(cx, opt, &ids, &mat_id) != SUCCESS)
		return (ERR_MALLOC);
	if (intern_surface(cx, opt, &surf_id, color) != SUCCESS)
		return (ERR_MALLOC);
	prim->material_id = mat_id;
	prim->surface_id = surf_id;
	return (SUCCESS);
}
