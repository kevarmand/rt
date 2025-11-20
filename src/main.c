/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 16:20:19 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/20 11:46:11 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "new_rt.h"
#include "errors.h"
#include "libft.h"

int	print_scene(const t_scene *scene); //provisoire

int main(int argc, char **argv)
{
	t_data	data;
	int		status;

	ft_memset(&data, 0, sizeof(t_data));
	status = check_args(argc, argv);
	if (status == SUCCESS)
		status = load_scene(argv[1], &data.scene);
	//En commentaire pour linstant car non implementer
	// if (status == SUCCESS)
	// 	status = run_app(&data);
	//test_provisoire :
	if (status == SUCCESS)
		print_scene(&data.scene);
	free_scene(&data);
	return (status);
}
