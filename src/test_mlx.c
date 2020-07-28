/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_mlx.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmand <karmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/03 18:33:55 by karmand           #+#    #+#             */
/*   Updated: 2020/03/03 18:57:41 by karmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		ft_init_win(t_data *data, char *name)
{
	if ((data->mlx_ptr = mlx_init()) == NULL)
		return (EXIT_FAILURE);
	if ((data->win_ptr = mlx_new_window(data->mlx_ptr,
		data->para.res_width, data->para.res_height, name)) == NULL)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int		ft_init_view(t_data *data)
{
	if (!(data->view = malloc(sizeof(t_image))))
		return (EXIT_FAILURE);
	if ((data->view->img_ptr = mlx_new_image(data->mlx_ptr, data->para.res_width,
		data->para.res_height)) == NULL)
		return (EXIT_FAILURE);
	if ((data->view->addr_ptr = mlx_get_data_addr(data->view->img_ptr,
		&data->view->bpp, &data->view->size_l, &data->view->endiant)) == NULL)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

