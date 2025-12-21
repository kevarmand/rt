/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intern_surface.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 13:23:52 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 06:59:54 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../convert.h"
#include "io.h"
#include "errors.h"
#include <stdlib.h>
#include "libft.h"
#include "scene.h"
#include "color.h"

//intern_surface

int	intern_surface(t_conv_ctx *cx,
		t_element_options *opt, t_index *out_surf, int *color)
{
	t_surface_map	new_surf;
	int				status;

	new_surf.map_uv[0] = opt->uv[0];
	new_surf.map_uv[1] = opt->uv[1];
	new_surf.map_uv[2] = opt->uv[2];
	new_surf.map_uv[3] = opt->uv[3];
	new_surf.map_uv[4] = opt->uv[4];
	new_surf.map_uv[5] = opt->uv[5];
	new_surf.texture_albedo_id = opt->texture_id;
	new_surf.bump_normal_id = opt->bumpmap_id;
	rgb8_to_linear_vec(color, &new_surf.color);
	new_surf.uv_mod = opt->uv_mod;
	new_surf.checker_mode = opt->checker_mode;
	new_surf.checker_texture_id = opt->checker_texture_id;
	new_surf.bump_scale = opt->bscale;
	rgb8_to_linear_vec(opt->checker_color, &new_surf.checker_color);
	status = vector_push_back(&cx->surf_v, &new_surf);
	if (status < 0)
		return (perr(ERR_MALLOC, PERR_M_VEC_PUSH));
	*out_surf = (vector_size(&cx->surf_v) - 1);
	return (SUCCESS);
}
