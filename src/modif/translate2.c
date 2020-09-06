/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   translate2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmand <karmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 16:16:15 by karmand           #+#    #+#             */
/*   Updated: 2020/03/05 17:09:10 by karmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_vector	translate_type(int type)
{
	t_vector	v;

	v.x = 0;
	v.y = 0;
	v.z = 0;
	if (type == AXE_XP)
		v.x = VITESSE_TRANSLAT;
	if (type == AXE_YP)
		v.y = VITESSE_TRANSLAT;
	if (type == AXE_ZP)
		v.z = VITESSE_TRANSLAT;
	if (type == AXE_XM)
		v.x = -VITESSE_TRANSLAT;
	if (type == AXE_YM)
		v.y = -VITESSE_TRANSLAT;
	if (type == AXE_ZM)
		v.z = -VITESSE_TRANSLAT;
	return (v);
}

int			translate_light(t_light *light, int type)
{
	t_vector	v;

	v = translate_type(type);
	light->point = vect_translate(&(light->point), &v);
	return (SUCCESS);
}

int			translate_cam(t_cam *cam, int type)
{
	t_vector	v;

	v = translate_type(type);
	cam->point = vect_translate(&(cam->point), &v);
	return (SUCCESS);
}

int			translate_switch(int type, t_data *data)
{
	t_list	*p;

	p = ((data->select).obj);
	if (p->type == PLANE)
		return (translate_plane((t_pl *)(p->obj), type));
	if (p->type == SPHERE)
		return (translate_sphere((t_sp *)(p->obj), type));
	if (p->type == TRIANGLE)
		return (translate_triangle((t_tr *)(p->obj), type));
	if (p->type == CYLINDER)
		return (translate_cylinder((t_cy *)(p->obj), type));
	if (p->type == SQUARE)
		return (translate_square((t_sq *)(p->obj), type));
	return (0);
}
