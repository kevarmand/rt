/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   select.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmand <karmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 16:16:15 by karmand           #+#    #+#             */
/*   Updated: 2020/03/05 17:09:10 by karmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		select_init(t_data *data)
{
	(data->select).obj = data->lobj;
	(data->select).light = data->llight;
	(data->select).cam = data->lcam;
	data->select.active_tab = UI_TAB_OBJECT;
	return (0);
}

int		select_next_obj(t_data *data)
{
	if (((data->select).obj)->next == 0)
		(data->select).obj = data->lobj;
	else
		((data->select).obj) = ((data->select).obj)->next;
	return (0);
}

int		select_next_light(t_data *data)
{
	if (((data->select).light)->next == 0)
		(data->select).light = data->llight;
	else
		((data->select).light) = ((data->select).light)->next;
	return (0);
}

int		select_next_cam(t_data *data)
{
	if (((data->select).cam)->next == 0)
		(data->select).cam = data->lcam;
	else
		((data->select).cam) = ((data->select).cam)->next;
	return (0);
}
