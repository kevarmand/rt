#include "render.h"
#include "rt.h"


void assign_color_to_buffer(int *buf, int index, const t_color *co)
{
	buf[index] = co->r;
	buf[index + 1] = co->g;
	buf[index + 2] = co->b;
}

int			render_tile(t_data *data, t_tile *tile, t_cam *cam)
{
	int			i;
	int			j;
	t_point		pf;
	t_vector	v;
	t_color		co;

	int			x0, y0;


	tile_index_to_pos(tile->id, data->para.res_width, &x0, &y0);
	i = -1;
	while (++i < data->para.res_width)
	{
		v = vect_mult(&(cam->hori), i);
		pf = vect_translate(&(cam->first), &v);
		j = -1;
		while (++j < data->para.res_height)
		{
			v = vect_vector(&(cam->point), &pf);
			vect_tonorm(&v);
			if (i >= x0 && i < x0 + TILE_SIZE && j >= y0 && j < y0 + TILE_SIZE)
			{
				co = cam_getcolor(data, cam, &v);
				assign_color_to_buffer(tile->buffer, (i - x0 + (j - y0) * TILE_SIZE) * RGB_CHANNELS, &co);
			}
			pf = vect_translate(&pf, &(cam->vert));
		}
	}
	return (SUCCESS);
}