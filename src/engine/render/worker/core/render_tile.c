/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_tile.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 22:19:54 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/18 15:07:35 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include "engine.h"
#include "scene.h"
#include "rt_config.h"
#include "new_rt.h"
#include "errors.h"
#include <stdio.h>

static t_vec3f	calcul_pixel_color(t_data *data, const t_render_view *view,
			int img_x, int img_y)
{
	t_ray			ray;
	t_shading_ctx	ctx;
	t_vec3f			color_out;

	ctx.depth = 0;
	ctx.contribution = 1.0f;
	ctx.current_ior = 1.0f;
	build_ray_for_pixel(view, (float)img_x, (float)img_y, &ray);
	shading_ray(&data->scene, &ray, &ctx, &color_out);
	return (color_out);
}

int	render_tile_normal(t_data *data, t_tile *tile, const t_render_view *view)
{
	t_vec3f	color;
	int		local_y;
	int		local_x;
	int		index;

	local_y = 0;
	index = 0;
	while (local_y < tile->pixel_height)
	{
		local_x = 0;
		while (local_x < tile->pixel_width)
		{
			color = calcul_pixel_color(data, view,
					tile->pixel_x + local_x,
					tile->pixel_y + local_y);
			tile->hdr_pixels[index] = color;
			index++;
			local_x++;
		}
		local_y++;
	}
	return (SUCCESS);
}
