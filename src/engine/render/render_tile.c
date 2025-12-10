#include "render.h"
//SURTOUT PAS INCLURE RT.H ICI
#include "engine.h"
#include "scene.h"
#include "rt_config.h"
#include "new_rt.h"
#include "errors.h"

#include <stdio.h>





static int	calcul_pixel_color0(t_data *data, const t_render_view *view,
			int img_x, int img_y, t_vec3f *out_color)
{
	t_ray	ray;
	t_shading_ctx	ctx;

	ctx.depth = 0;
	ctx.contribution = 1.0f;
	ctx.current_ior = 1.0f;
	build_ray_for_pixel(view, img_x, img_y, &ray);
	shading_ray(&data->scene, &ray, &ctx, out_color);
	return (SUCCESS);
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
			calcul_pixel_color0(data, view,
				tile->pixel_x + local_x,
				tile->pixel_y + local_y, &color);
			tile->hdr_pixels[index] = color;
			index++;
			local_x++;
		}
		local_y++;
	}
	return (SUCCESS);
}

static int	calcul_pixel_color1(t_data *data, const t_render_view *view,
			int img_x, int img_y, t_vec3f *out_color)
{
	t_ray	ray;
	t_shading_ctx	ctx;

	ctx.depth = 0;
	ctx.contribution = 1.0f;
	ctx.current_ior = 1.0f;
	build_ray_for_pixel(view, img_x, img_y, &ray);
	shading_ray(&data->scene, &ray, &ctx, out_color);
	return (SUCCESS);
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
			calcul_pixel_color1(data, view,
				tile->pixel_x + local_x,
				tile->pixel_y + local_y, &color);
			tile->hdr_pixels[index] = color;
			index++;
			local_x++;
		}
		local_y++;
	}
	return (SUCCESS);
}