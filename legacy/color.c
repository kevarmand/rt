/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmand <karmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/02 14:53:22 by karmand           #+#    #+#             */
/*   Updated: 2020/03/05 21:14:06 by karmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_color		color_ray(t_color *c1, t_color *c2)
{
	t_color	c;

	c.r = c1->r * c2->r / 255;
	c.g = c1->g * c2->g / 255;
	c.b = c1->b * c2->b / 255;
	return (c);
}

int			color_add(t_lighting *li, t_ray *r, t_light *l)
{
	double	a;
	t_color	c;

	vect_tonorm(&(r->dir));
	vect_tonorm(&(li->norm));
	a = vect_dot(&(li->norm), &(r->dir));
	if (a <= 0)
		a = -a;
	c = color_ray(&(li->c_obj), &(l->color));
	light_add(li, &c, a);
	return (SUCCESS);
}

int			color_s2(t_ray *r, t_vec3 *v, t_lighting *li, t_data *data)
{
	int		inter;

	inter = r->inter;
	(*v) = (r->dir);
	r->dir = vect_mult(&(r->dir), r->dist);
	r->source = vect_translate(&(r->source), &(r->dir));
	li->inter = r->source;
	light_init(li, &((data->ldist.plot)[inter]), &(data->para.color),
		(data->para.brightness));
	return (inter);
}

t_color		color_switch(t_data *data, t_ray *r)
{
	t_objlist		*l;
	t_vec3	v;
	t_lighting	li;
	int			inter;
	double		sign[2];

	l = data->llight;
	inter = color_s2(r, &v, &li, data);
	sign[0] = (vect_dot(&(li.norm), &v) > 0) ? 1 : -1;
	while (l != 0)
	{
		r->dir = vect_vector(&(r->source), &((t_light *)(l->obj))->point);
		inter_inter(r, &(data->ldist), inter);
		if (!(r->dist < 1 && r->dist > 0.001))
		{
			sign[1] = (vect_dot(&(li.norm), &(r->dir)) >= 0) ? 1 : -1;
			if (sign[0] != sign[1])
				color_add(&li, r, (t_light *)(l->obj));
		}
		l = l->next;
	}
	return (li.c_ret);
}
