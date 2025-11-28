/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotate2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmand <karmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 16:16:15 by karmand           #+#    #+#             */
/*   Updated: 2020/03/05 17:09:10 by karmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	rot_p2(t_vec3 *v, int type, t_vec3 *w)
{
	if (type == AXE_YM)
	{
		w->x = COS_VR * v->x + SIN_VR * v->z;
		w->y = v->y;
		w->z = COS_VR * v->z - SIN_VR * v->x;
	}
	if (type == AXE_ZP)
	{
		w->x = COS_VR * v->x + SIN_VR * v->y;
		w->y = COS_VR * v->y - SIN_VR * v->x;
		w->z = v->z;
	}
	if (type == AXE_ZM)
	{
		w->x = COS_VR * v->x - SIN_VR * v->y;
		w->y = COS_VR * v->y + SIN_VR * v->x;
		w->z = v->z;
	}
}

int		rot_vect(t_vec3 *v, int type)
{
	t_vec3	u;

	if (type == AXE_XP)
	{
		u.x = v->x;
		u.y = COS_VR * v->y + SIN_VR * v->z;
		u.z = COS_VR * v->z - SIN_VR * v->y;
	}
	if (type == AXE_XM)
	{
		u.x = v->x;
		u.y = COS_VR * v->y - SIN_VR * v->z;
		u.z = COS_VR * v->z + SIN_VR * v->y;
	}
	if (type == AXE_YP)
	{
		u.x = COS_VR * v->x - SIN_VR * v->z;
		u.y = v->y;
		u.z = COS_VR * v->z + SIN_VR * v->x;
	}
	rot_p2(v, type, &u);
	vect_tonorm(&u);
	(*v) = u;
	return (SUCCESS);
}
