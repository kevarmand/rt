/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmand <karmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/10 14:40:51 by karmand           #+#    #+#             */
/*   Updated: 2020/02/19 17:48:30 by karmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	data_init(t_data *data)
{
	data->para.res_width = 0;
	data->para.res_height = 120;
	data->para.brightness = 0;
	data->para.color.r = 0;
	data->para.color.g = 0;
	data->para.color.b = 0;
	data->lobj = NULL;
	data->llight = 0;
	data->lcam = 0;
	(data->test)[0] = 0;
	(data->test)[1] = 0;
	(data->test)[2] = 0;
}
