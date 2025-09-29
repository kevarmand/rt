/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmand <karmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 17:26:08 by karmand           #+#    #+#             */
/*   Updated: 2020/03/05 23:32:35 by karmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		init_light(t_light *l)
{
	l->color.r = (int)((double)(l->color.r) * l->brightness);
	l->color.g = (int)((double)(l->color.g) * l->brightness);
	l->color.b = (int)((double)(l->color.b) * l->brightness);
	return (SUCCESS);
}

int		init_cam_2(t_cam *c, t_data *data)
{
	t_point		p;
	t_vec3	v;

	p = c->point;
	p = vect_translate(&p, &(c->dir));
	v = vect_mult(&(c->hori), -(data->para.res_width - 1) / 2);
	p = vect_translate(&p, &v);
	v = vect_mult(&(c->vert), -(data->para.res_height - 1) / 2);
	p = vect_translate(&p, &v);
	c->first = p;
	return (SUCCESS);
}

int		init_cam(t_cam *c, t_data *data)
{
	t_vec3	v;

	v = vect_def(0, 0, 1);
	if (vect_is_col(&(c->dir), &v))
	{
		c->hori = vect_def(1, 0, 0);
		c->vert = vect_def(0, 1, 0);
	}
	else
	{
		v = vect_def(0, 0, 1);
		c->hori = vect_vect(&(c->dir), &v);
		c->vert = vect_vect(&(c->dir), &(c->hori));
	}
	vect_tonorm(&(c->hori));
	vect_tonorm(&(c->vert));
	vect_tonorm(&(c->dir));
	c->dir = vect_mult(&(c->dir), ((double)data->para.res_width - 1.0)
		/ (2.0 * tan(c->fov / 2.0)));
	return (init_cam_2(c, data));
}
