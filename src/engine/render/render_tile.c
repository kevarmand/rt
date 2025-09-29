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
	t_vec3	v;
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


/***
 * Fonction de rendu
 * dans l'idée j'aimerais avoir cette ossature :
 *  le nom//paramettre de la fonction sont ok
 * int			render_tile(t_data *data, t_tile *tile, t_cam *cam)
{
	//Block de variables


	//fonction d initialisation des valeur necessaire
	i = -1;
	while (++i < TILE_SIZE)
	{
		j = -1;
		while (++j < TILE_SIZE)
		{
			//calcul de la direction du rayon une sorte de ray_init
			//ray_init(&r, &cam->point, &v, ...);
			ray_trace(&r, data, &co);
			assign_color_to_buffer(tile->buffer, (i + j * TILE_SIZE) * RGB_CHANNELS, &co);
		}
	}
	return (SUCCESS);
}
 * ATTENTION je ne veux aps le faire tout de suite je t indique juste ce que je veux faire :
 *  je n ai pas encore fait le point sur les struct que je vqis qvoir besoin donc pour linstant je touche le moins possible au code
 * 
 * Donc la struct ray je la ferai plus tard
 * la fonction ray_trace le nom me semble significatif mais si tu peux proposer mieux n hésite pas
 * la fonction assign_color_to_buffer est une fonction facile qui ne nepend que d elimplementation du reste donc pas besoin d epasser du temps dessus
 */

 