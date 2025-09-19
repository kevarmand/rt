/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cam_raytracing.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 16:16:15 by karmand           #+#    #+#             */
/*   Updated: 2025/09/17 17:03:46 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_color		cam_getcolor(t_data *data, t_cam *c, t_vector *v)
{
	t_ray		r;
	t_color		co;

	r.source = c->point;
	r.dir = *v;
	inter_inter(&r, &(data->ldist), -1);
	if (r.inter == -1)
	{
		co.r = 135;
		co.g = 206;
		co.b = 235;
	}
	else
		co = color_switch(data, &r);
	return (co);
}

int			cam_gen(t_data *data, t_cam *c)
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
			co = cam_getcolor(data, c, &v);
			*(int *)(data->view->addr_ptr + ((j * data->para.res_width + i) *
			data->view->bpp / 8)) = 256 * 256 * co.r + 256 * co.g + co.b; //= co.r << 16 | co.g << 8 | co.b;
			pf = vect_translate(&pf, &(c->vert));
		}
	}
	return (SUCCESS);
}
