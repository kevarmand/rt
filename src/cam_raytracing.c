/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cam_raytracing.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmand <karmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 16:16:15 by karmand           #+#    #+#             */
/*   Updated: 2020/03/05 17:09:10 by karmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_color		cam_getcolor(t_data *data, t_cam *c, t_vector *v)
{
	t_ray		r;
	t_color		co;

	r.source = c->point;
	r.dir = *v;
	//Recherche de lintersection
	inter_inter(&r, &(data->ldist), -1);
	//printf ("   objet(%i)-dist = %f  ",r.inter, r.dist);
	//faut recup la vrai couleur de lobjet.
	
	if (r.inter == -1)
	{
		//peut etre faire une micro fonction initialisation
		co.r = 135;
		co.g = 206;
		co.b = 235;
	}
	else
	{
		co = color_switch(data, &r);	
	}
	
	//e ton fait partir 2 rayon ! pour la transparance et un pour la reflexion
	return (co);	
}

int		cam_gen(t_data *data, t_cam *c)
{
	int			i;
	int			j;
	t_point		pf;
	t_vector	v;
	t_color		co;

	i = -1;
	while (++i < data->para.res_width)
	{
		v = vect_mult(&(c->hori), i);
		pf = vect_translate(&(c->first), &v);
		j = -1;
		while (++j < data->para.res_height)
		{
			v = vect_vector(&(c->point), &pf);
			vect_tonorm(&v);

			//test_print_point((t_point *)&v);
			//printf("      +> i = %i, j = %i    ", i,j);
			//il nous faut une couleur
			co = cam_getcolor(data, c, &v);
			//test_print_color(&co);
			*(int *)(data->view->addr_ptr + ((j * data->para.res_width + i) *
			data->view->bpp/8 )) = 256*256*co.r + 256*co.g + co.b;

			//printf("\n");
			//incrementation
			pf = vect_translate(&pf, &(c->vert));
		}
	}
	return (SUCCESS);
}
