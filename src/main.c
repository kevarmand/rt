/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 16:20:19 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/16 10:04:35 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "new_rt.h"
#include "errors.h"
#include "libft.h"

int	print_scene(const t_scene *scene); //provisoire

void	free_data(t_data *data)
{
	(void)data;
	//free_engine(&data->engine); //à implémenter plus tard
	//free_display(&data->display); //à implémenter plus tard
}

int main(int argc, char **argv)
{
	t_data	data;
	int		status;

	ft_memset(&data, 0, sizeof(t_data));
	status = check_args(argc, argv);
	if (status == SUCCESS)
		status = load_scene(argv[1], &data.scene);
	if (status == SUCCESS)
	 	print_scene(&data.scene);
		if (status == SUCCESS)
		status = run_app(&data);
	//test_provisoire :

	free_data(&data);
	return (status);
}
