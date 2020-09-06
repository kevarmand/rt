/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resize.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmand <karmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 16:16:15 by karmand           #+#    #+#             */
/*   Updated: 2020/03/05 17:09:10 by karmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		resize_double(double *d, int type)
{
	if (type == PLUS)
		(*d) *= VITESSE_RESIZE;
	else
		(*d) /= VITESSE_RESIZE;
	return (0);
}

int		resize_switch1(int type, t_data *data)
{
	t_list	*p;

	p = (data->select).obj;
	if (p->type == CYLINDER)
		return (resize_double(&(((t_cy *)(p->obj))->rayon), type));
	if (p->type == SPHERE)
		return (resize_double(&(((t_sp *)(p->obj))->rayon), type));
	return (0);
}

int		resize_switch2(int type, t_data *data)
{
	t_list	*p;

	p = (data->select).obj;
	if (p->type == CYLINDER)
		return (resize_double(&(((t_cy *)(p->obj))->height), type));
	if (p->type == SQUARE)
		return (resize_double(&(((t_sq *)(p->obj))->height), type));
	return (0);
}
