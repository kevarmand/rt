/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lighting.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmand <karmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/02 18:22:53 by karmand           #+#    #+#             */
/*   Updated: 2020/03/05 23:27:04 by karmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		light_tr(t_lighting *li, t_tr *tr)
{
	li->c_obj = tr->color;
	li->norm = tr->norm;
	return (SUCCESS);
}

int		light_pl(t_lighting *li, t_pl *pl)
{
	li->c_obj = pl->color;
	li->norm = pl->norm;
	return (SUCCESS);
}

int		light_sp(t_lighting *li, t_sp *sp, t_point *p)
{
	li->c_obj = sp->color;
	li->norm = vect_vector(&(sp->centre), p);
	vect_tonorm(&(li->norm));
	return (SUCCESS);
}

int		light_ci(t_lighting *li, t_ci *pl)
{
	li->c_obj = pl->color;
	li->norm = pl->norm;
	return (SUCCESS);
}

int		light_add(t_lighting *li, t_color *color, double a)
{
	li->c_ret.r += (int)(a * (double)(color->r));
	if (li->c_ret.r > 255)
		li->c_ret.r = 255;
	li->c_ret.g += (int)(a * (double)(color->g));
	if (li->c_ret.g > 255)
		li->c_ret.g = 255;
	li->c_ret.b += (int)(a * (double)(color->b));
	if (li->c_ret.b > 255)
		li->c_ret.b = 255;
	return (SUCCESS);
}

int		light_init(t_lighting *li, t_plot *plot, t_color *color, double b)
{
	t_type	type;
	int		err;

	type = plot->type;

	if (type == PLANE)
	{
		if ((err = light_pl(li, (t_pl *)(plot->p))))
			return (err);
	}
	if (type == SPHERE)
	{
		if ((err = light_sp(li, (t_sp *)(plot->p), &(li->inter))))
			return (err);
	}
	if (type == TRIANGLE)
	{
		if ((err = light_tr(li, (t_tr *)(plot->p))))
			return (err);
	}
	if (type == CIRCLE)
	{
		if ((err = light_ci(li, (t_ci *)(plot->p))))
			return (err);
	}
	vect_tonorm(&(li->norm));
	li->c_ret.r = color->r * b * li->c_obj.r / 255;
	li->c_ret.g = color->g * b * li->c_obj.g / 255;
	li->c_ret.b = color->b * b * li->c_obj.b / 255;
	return (SUCCESS);
}
