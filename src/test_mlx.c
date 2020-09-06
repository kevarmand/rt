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
	int		m;
	int		n;

	if ((data->mlx_ptr = mlx_init()) == NULL)
		return (ERR_MLX);
	mlx_get_screen_size(data->mlx_ptr, &m, &n);
	data->para.res_width = (data->para.res_width > m) ?
		m : data->para.res_width;
	data->para.res_height = (data->para.res_height > n) ?
		n : data->para.res_height;
	if ((data->win_ptr = mlx_new_window(data->mlx_ptr,
		data->para.res_width, data->para.res_height, name)) == NULL)
		return (ERR_MLX);
	return (SUCCESS);
}

int		ft_init_view(t_data *data)
{
	if (!(data->view = malloc(sizeof(t_image))))
		return (ERR_MLX);
	if ((data->view->img_ptr = mlx_new_image(data->mlx_ptr,
		data->para.res_width, data->para.res_height)) == NULL)
		return (ERR_MLX);
	if ((data->view->addr_ptr = mlx_get_data_addr(data->view->img_ptr,
		&data->view->bpp, &data->view->size_l, &data->view->endiant)) == NULL)
		return (ERR_MLX);
	return (SUCCESS);
}

int		ft_init_bmp(t_data *data, char *name)
{
	if ((data->mlx_ptr = mlx_init()) == NULL)
	{
		error(ERR_MLX);
		return (ft_data_exit(data) + dist_destroy(&(data->ldist)));
	}
	if (ft_init_view(data))
	{
		error(ERR_MLX);
		return (ft_data_exit(data) + dist_destroy(&(data->ldist)));
	}
	select_init(data);
	cam_gen(data, (t_cam *)(((data->select).cam)->obj));
	bmp(data, name);
	ft_exit_bmp(data);
	return (0);
}
