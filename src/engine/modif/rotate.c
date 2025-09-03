/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotate.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmand <karmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 16:16:15 by karmand           #+#    #+#             */
/*   Updated: 2020/03/05 17:09:10 by karmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		rotate_plane(t_pl *pl, int type)
{
	rot_vect(&(pl->norm), type);
	norm_pl(pl);
	return (SUCCESS);
}

int		rotate_square(t_sq *sq, int type)
{
	rot_vect(&(sq->norm), type);
	rot_vect(&(sq->dir1), type);
	rot_vect(&(sq->dir2), type);
	norm_sq(sq);
	return (SUCCESS);
}

int		rotate_cylinder(t_cy *cy, int type)
{
	rot_vect(&(cy->dir), type);
	return (SUCCESS);
}

int		rotate_cam(t_cam *cam, int type)
{
	rot_vect(&(cam->dir), type);
	return (SUCCESS);
}

int		rotate_switch(int type, t_data *data)
{
	t_list	*p;

	p = (data->select).obj;
	if (p->type == PLANE)
		return (rotate_plane((t_pl *)(p->obj), type));
	if (p->type == CYLINDER)
		return (rotate_cylinder((t_cy *)(p->obj), type));
	if (p->type == SQUARE)
		return (rotate_square((t_sq *)(p->obj), type));
	return (0);
}
