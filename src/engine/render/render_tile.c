#include "render.h"
//SURTOUT PAS INCLURE RT.H ICI
#include "engine.h"
#include "scene.h"
#include "rt_config.h"
#include "new_rt.h"
#include "errors.h"

#include <stdio.h>

static void	build_ray_for_pixel(const t_render_view *view,
			int img_x, int img_y, t_ray *ray)
{
	t_vec3f	offset_x;
	t_vec3f	offset_y;
	t_vec3f	pixel_pos;
	t_vec3f	dir;

	offset_x = vec3f_scale(view->dx, (float)img_x);
	offset_y = vec3f_scale(view->dy, (float)img_y);
	pixel_pos = vec3f_add(view->p0, offset_x);
	pixel_pos = vec3f_add(pixel_pos, offset_y);
	dir = vec3f_sub(pixel_pos, view->origin);
	dir = vec3f_normalize(dir);
	ray->origin = view->origin;
	ray->dir = dir;
	ray->invdir = 1 / dir;
	ray->orig_div = view->origin * ray->invdir;
	ray->sign[0] = (ray->invdir.x < 0);
	ray->sign[1] = (ray->invdir.y < 0);
	ray->sign[2] = (ray->invdir.z < 0);
}

static int	calcul_pixel_color(t_data *data, const t_render_view *view,
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

int	render_tile(t_data *data, t_tile *tile, const t_render_view *view)
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
			calcul_pixel_color(data, view,
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
