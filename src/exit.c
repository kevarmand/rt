/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmand <karmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 16:16:15 by karmand           #+#    #+#             */
/*   Updated: 2020/03/05 17:09:10 by karmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		ft_data_exit(t_data *data)
{
	if (data->lobj != 0)
		list_free(data->lobj);
	data->lobj = 0;
	if (data->llight != 0)
		list_free(data->llight);
	data->llight = 0;
	if (data->lcam != 0)
		list_free(data->lcam);
	data->lcam = 0;
	return (0);
}

int		ft_exit(t_data *data)
{
	ft_data_exit(data);
	dist_destroy(&(data->ldist));
	data->test[0] = 1;
	mlx_destroy_image(data->mlx_ptr, data->view->img_ptr);
	mlx_destroy_window(data->mlx_ptr, data->win_ptr);
	exit(0);
}

int		ft_exit_bmp(t_data *data)
{
	ft_data_exit(data);
	dist_destroy(&(data->ldist));
	data->test[0] = 1;
	mlx_destroy_image(data->mlx_ptr, data->view->img_ptr);
	exit(0);
}
