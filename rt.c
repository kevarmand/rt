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
	if (ac < 2 && ac > 3)
		return (ERROR_ARGUMENT);
	if (ac == 3 && 0 != ft_strcmp(av[2], "-save"))
		return (ERROR_ARGUMENT);
	return (SUCCESS);
}

static int	open_file(int *fd, char *name)
{
	*fd = open(name, O_RDONLY);
	if (*fd == -1)
		return (ERROR_OPEN);	
	return (SUCCESS);
}

int main(int ac, char **av)
{
	int err;
	int fd;
	t_data data;

	if ((err = test_param(ac, av)))
		return (error(err));
	if ((err = open_file(&fd, av[1])))
		return (error(err));
	data_init(&data);

	if ((err = parsing(fd, &data)))
		printf("\n\nError %i\n\n", err);

	ft_test_circle(&data);
	printf("TEST1\n");
	test_data_print(&data);	//Fonction de test
	if ((err = transform(data.lobj)))
		printf("\n\nError %i\n\n", err);
	
	printf("TEST2\n");
	if ((err = transform_cam(data.lcam, &data)))
		printf("\n\nError %i\n\n", err);
	if ((err = transform_light(data.llight)))
		printf("\n\nError %i\n\n", err);

	printf("TEST3\n");
	dist_init(&data, &(data.ldist));
	test_data_print(&data);	//Fonction de test
	test_print_dist(&(data.ldist));
	
	printf("TEST4\n");
		
	//dsklflkds test mlx;
	t_image		image;
	ft_init_win(&data);
	ft_init_view(&data);

	printf("TEST5\n");
	
	cam_gen(&data, (t_cam *)((data.lcam)->obj));	//fonction de test : on est un cam
	
	mlx_put_image_to_window(data.mlx_ptr, data.win_ptr,
	data.view->img_ptr, 0, 0);
	mlx_loop(data.mlx_ptr);
	if (err != SUCCESS)
		return (error(err));
	return (0);
}
