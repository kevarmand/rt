/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_super.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 10:20:00 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/18 15:48:58 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include "render.h"
#include "scene.h"
#include "rt_config.h"
#include "vector.h"
#include "errors.h"
#include "new_rt.h"

static t_vec3f	trace_sample_full(t_data *data,
			const t_render_view *view, float fx, float fy)
{
	t_ray			ray;
	t_shading_ctx	ctx;
	t_vec3f			color;

	ctx.depth = 0;
	ctx.contribution = 1.0f;
	ctx.current_ior = 1.0f;
	build_ray_for_pixel(view, fx, fy, &ray);
	shading_ray(&data->scene, &ray, &ctx, &color);
	return (color);
}

static t_vec3f	calcul_pixel_color_super(t_data *data,
			const t_render_view *view, int x, int y)
{
	t_vec3f	accum;
	float	offset;
	int		k;

	accum = (t_vec3f){0, 0, 0};
	offset = 1.0f / 3.0f;
	k = 0;
	while (k < 9)
	{
		accum = vec3f_add(accum,
				trace_sample_full(data, view,
					(float)x + (((k % 3) - 1) * offset),
					(float)y + (((k / 3) - 1) * offset)));
		k++;
	}
	return (vec3f_scale(accum, 1.0f / 9.0f));
}

int	render_tile_super(t_data *data, t_tile *tile, const t_render_view *view)
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
			color = calcul_pixel_color_super(data, view,
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
