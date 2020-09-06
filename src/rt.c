/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmand <karmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/10 13:59:23 by karmand           #+#    #+#             */
/*   Updated: 2020/03/05 23:28:45 by karmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static int	test_param(int ac, char **av)
{
	if (ac == 2)
		return (SUCCESS);
	if (ac == 3 && 0 == ft_strcmp(av[2], "-save"))
		return (SUCCESS);
	return (ERR_ARG);
}

static int	open_file(int *fd, char *name)
{
	char **tab;

	if (!(tab = ft_split(name, ".")))
		return (free_split(tab) + ERR_EXT);
	if (split_count(tab) == 2 && 0 == ft_strcmp("rt", tab[1]))
	{
		*fd = open(name, O_RDONLY);
		if (*fd == -1)
			return (free_split(tab) + ERR_OPEN);
		return (free_split(tab) + SUCCESS);
	}
	return (free_split(tab) + ERR_EXT);
}

int			testcast(t_data *data)
{
	if (data->test[0] == 0)
		mlx_put_image_to_window(data->mlx_ptr, data->win_ptr,
	(data->view)->img_ptr, 0, 0);
	return (0);
}

int			ft_mlx_init(t_data *data, char *name)
{
	if (ft_init_win(data, name))
	{
		error(ERR_MLX);
		ft_data_exit(data);
		dist_destroy(&(data->ldist));
		return (0);
	}
	if (ft_init_view(data))
	{
		error(ERR_MLX);
		ft_data_exit(data);
		dist_destroy(&(data->ldist));
		mlx_destroy_image(data->mlx_ptr, data->view->img_ptr);
		return (0);
	}
	select_init(data);
	mlx_hook(data->win_ptr, 33, StructureNotifyMask, &ft_exit, data);
	mlx_key_hook(data->win_ptr, &key_hook, data);
	mlx_loop_hook(data->mlx_ptr, &testcast, data);
	render(data);
	mlx_loop(data->mlx_ptr);
	return (0);
}

int			main(int ac, char **av)
{
	int		err;
	int		fd;
	t_data	data;

	data_init(&data);
	if ((err = test_param(ac, av)))
		return (error(err));
	if ((err = open_file(&fd, av[1])))
		return (err_op(err, av[1]));
	if ((err = parsing(fd, &data)))
		return (close(fd) + ft_data_exit(&data));
	close(fd);
	if (data.lobj == 0 || data.llight == 0 || data.lcam == 0 ||
		(data.test)[1] == 0 || (data.test)[2] == 0)
		return (error(ERR_MIN) + ft_data_exit(&data));
	if (dist_init(&data, &(data.ldist)))
		return (0);
	if (ac == 3)
		return (ft_init_bmp(&data, av[1]));
	return (ft_mlx_init(&data, av[1]));
}
