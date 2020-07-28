/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transf.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmand <karmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 17:26:08 by karmand           #+#    #+#             */
/*   Updated: 2020/03/05 23:32:35 by karmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		transf_ci(t_ci *ci)
{
	//error si a = b = c = 0;)
	ci->a = ci->norm.x;
	ci->b = ci->norm.y;
	ci->c = ci->norm.z;
	if (ci->a == 0 && ci->b == 0 && ci->c == 0)
		return (ERROR_PLANE_NORM);
	ci->d = -(ci->a * ci->centre.x);
	ci->d -= (ci->b * ci->centre.y);
	ci->d -= (ci->c * ci->centre.z);
	return (SUCCESS);
}

int		transf_pl(t_pl *pl)
{
	//error si a = b = c = 0;)
	pl->a = pl->norm.x;
	pl->b = pl->norm.y;
	pl->c = pl->norm.z;
	if (pl->a == 0 && pl->b == 0 && pl->c == 0)
		return (ERROR_PLANE_NORM);
	pl->d = -(pl->a * pl->point.x);
	pl->d -= (pl->b * pl->point.y);
	pl->d -= (pl->c * pl->point.z);
	return (SUCCESS);
}

int		transf_sp(t_sp *sp)
{
	return (SUCCESS);
}

int		transf_tr(t_tr *tr)
{
	t_vector	v1;
	t_vector	v2;
	
	v1 = vect_vector(&(tr->p1), &(tr->p2));
	v2 = vect_vector(&(tr->p1), &(tr->p3));
	tr->norm = vect_vect(&v1, &v2);
	tr->a = tr->norm.x;
	tr->b = tr->norm.y;
	tr->c = tr->norm.z;
	if (tr->a == 0 && tr->b == 0 && tr->c == 0)
		return (ERROR_PLANE_NORM);
	tr->d = -(tr->a * tr->p1.x);
	tr->d -= (tr->b * tr->p1.y);
	tr->d -= (tr->c * tr->p1.z);
	return (SUCCESS);
}

int		transf_cam_first(t_cam *c, t_data *data)
{
	t_point		p;
	t_vector	v;

	p = c->point;
	p = vect_translate(&p, &(c->dir));
	v = vect_mult(&(c->hori), -(data->para.res_width - 1)/2);
	p = vect_translate(&p, &v);
	v = vect_mult(&(c->vert), -(data->para.res_height - 1)/2);	
	p = vect_translate(&p, &v);
	c->first = p;
	return (SUCCESS);
}

int		transf_cam(t_cam *c, t_data *data)
{
	t_vector	v;

	v = vect_def(0,0,1);
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
	c->dir = vect_mult(&(c->dir), ((double)data->para.res_width - 1.0) / (2.0 * tan(c->fov / 2.0)));
	return (transf_cam_first(c, data));
}

int		transf_switch(t_list *l)
{
	if (l->type == PLANE)
		return (transf_pl((t_pl *)(l->obj)));
	if (l->type == SPHERE)
		return (transf_sp((t_sp *)(l->obj)));
	if (l->type == TRIANGLE)
		return (transf_tr((t_tr *)(l->obj)));
	if (l->type == CIRCLE)
		return (transf_ci((t_ci *)(l->obj)));
	return (SUCCESS);
}

int		transform(t_list *l)
{
	int		err;

	while (l != 0)
	{
		if ((err = transf_switch(l)))
			return (err);
		l = l->next;
	}
	return (SUCCESS);
}

int		transform_cam(t_list *l, t_data *data)
{
	int		err;

	while (l != 0)
	{
		if ((err = transf_cam((t_cam *)(l->obj), data)))
			return (err);
		l = l->next;
	}
	return (SUCCESS);
}

int		transf_light(t_light *l)
{
	l->color.r =(int)((double)(l->color.r) * l->brightness);
	l->color.g =(int)((double)(l->color.g) * l->brightness);
	l->color.b =(int)((double)(l->color.b) * l->brightness);
	return (SUCCESS);
}

int		transform_light(t_list *l)
{
	int err;
	while (l != 0)
	{
		if ((err = transf_light((t_light *)(l->obj))))
			return (err);
		l = l->next;
	}
	return (SUCCESS);
}
