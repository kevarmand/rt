#include "render.h"
//SURTOUT PAS INCLURE RT.H ICI
#include "engine.h"
#include "scene.h"
#include "rt_config.h"
#include "new_rt.h"
#include "errors.h"

static void	build_ray_for_pixel(const t_cam_view *view,
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
}


static int	calcul_pixel_color(t_data *data, const t_cam_view *view,
			int img_x, int img_y, t_vec3f *out_color)
{
	t_ray	ray;

	build_ray_for_pixel(view, img_x, img_y, &ray);
	return (shade_ray(&data->scene, &ray, out_color));
}

int	render_tile(t_data *data, t_tile *tile)
{
	const t_cam_view	*view;
	t_vec3f				color;
	int					local_y;
	int					local_x;
	int					index;

	view = tile->cam_view;
	local_y = 0;
	index = -1;
	while (local_y < TILE_SIZE)
	{
		local_x = 0;
		while (local_x < TILE_SIZE)
		{
			calcul_pixel_color(data, view,
				tile->x + local_x, tile->y + local_y, &color);
			index++;
			tile->hdr_pixels[index] = color;
			local_x++;
		}
		local_y++;
	}
	return (SUCCESS);
}
