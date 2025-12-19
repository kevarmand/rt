/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_super.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 10:20:00 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/19 16:51:56 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include "render.h"
#include "scene.h"
#include "rt_config.h"
#include "vector.h"
#include "errors.h"
#include "new_rt.h"

typedef struct s_super_ctx
{
	int		ssaa;
	float	inv_ssaa;
	int		sx;;
	int		sy;
	float	fx;
	float	fy;
}	t_super_ctx;

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
	t_vec3f		accum;
	t_super_ctx	ctx;

	accum = (t_vec3f){0.0f, 0.0f, 0.0f};
	ctx.ssaa = view->ssaa;
	if (ctx.ssaa < 1)
		ctx.ssaa = 1;
	ctx.inv_ssaa = 1.0f / (float)ctx.ssaa;
	ctx.sy = 0;
	while (ctx.sy < ctx.ssaa)
	{
		ctx.sx = 0;
		while (ctx.sx < ctx.ssaa)
		{
			ctx.fx = (float)x + ((float)ctx.sx + 0.5f) * ctx.inv_ssaa;
			ctx.fy = (float)y + ((float)ctx.sy + 0.5f) * ctx.inv_ssaa;
			accum = vec3f_add(accum, trace_sample_full(data, view, ctx.fx,
					ctx.fy));
			ctx.sx++;
		}
		ctx.sy++;
	}
	return (vec3f_scale(accum, 1.0f / (float)(ctx.ssaa * ctx.ssaa)));
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
