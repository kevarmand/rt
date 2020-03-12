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
	//testavec lintersection
	c = color_ray(&(li->c_obj), &(l->color));
	light_add(li, &c, a);
	return (SUCCESS);
}

t_color		color_switch(t_data *data, t_ray *r)
{
	t_list		*l;
	t_vector 	v;
	t_lighting	li;
	int			inter;
	double		sign1;
	double		sign2;

	l = data->llight;
	inter = r->inter;
	
	
	v = (r->dir);
	r->dir = vect_mult(&(r->dir), r->dist);
	r->source = vect_translate(&(r->source), &(r->dir));
	li.inter = r->source;
	light_init(&li, &((data->ldist.plot)[inter]), &(data->para.color), (data->para.brightness));	
	sign1 = (vect_dot(&(li.norm), &v) > 0) ? 1: -1; 
	while (l != 0)
	{
		//test couleur directionelle ou non
		r->dir = vect_vector(&(r->source), &((t_light *)(l->obj))->point);
		inter_inter(r, &(data->ldist), inter);
		if (r->dist < 1 && r->dist > 0 && inter != r->inter)
		{
			//rien
		}
		else
		{	
			sign2 = (vect_dot(&(li.norm), &(r->dir)) > 0) ? 1 : -1;
			if (sign1 != sign2)
				color_add(&li, r, (t_light *)(l->obj));
		}
		l = l->next;
	}
	return (li.c_ret);
}
