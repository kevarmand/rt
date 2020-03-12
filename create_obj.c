/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_obj.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmand <karmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/05 22:59:19 by karmand           #+#    #+#             */
/*   Updated: 2020/03/05 23:37:15 by karmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"


int		create_circle(t_data *data)
{
	int		err;
	t_ci	*ci;

	if (!(ci = malloc(sizeof(t_ci))))
		return (ERROR_MALLOC);
	if ((err = list_addfront(&(data->lobj), (void *)ci, CIRCLE)))
		return (ERROR_MALLOC);
	return (SUCCESS);
}


int		ft_test_circle(t_data *data)
{
	t_vector v;
	t_color c;
	t_point p;
	p.x = 5;
	p.y = 5;
	p.z = 0;
	c.r = 255;
	c.g = 100;
	c.b = 0;
	v.x = -0.2;
	v.y = -0.2;
	v.z = 1;
	int		err;
	t_ci	*ci;

	if (!(ci = malloc(sizeof(t_ci))))
		return (ERROR_MALLOC);
	if ((err = list_addfront(&(data->lobj), (void *)ci, CIRCLE)))
		return (ERROR_MALLOC);
	ci->color = c;
	ci->rayon = 3;
	ci->centre = p;
	ci->norm = v;
	return (SUCCESS);
}
