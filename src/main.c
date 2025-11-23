/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 16:20:19 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/23 12:33:29 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "new_rt.h"
#include "errors.h"
#include "libft.h"

int	print_scene(const t_scene *scene); //provisoire

void	free_scene(t_scene *scene)
{
	if (scene->primitives)
		free(scene->primitives);
	if (scene->planes)
		free(scene->planes);
	if (scene->cameras)
		free(scene->cameras);
	if (scene->lights)
		free(scene->lights);
	if (scene->surfaces)
		free(scene->surfaces);
	if (scene->textures)
		free(scene->textures);
	if (scene->materials)
		free(scene->materials);
}

int main(int argc, char **argv)
{
	t_data	data;
	int		status;

	ft_memset(&data, 0, sizeof(t_data));
	status = check_args(argc, argv);
	if (status == SUCCESS)
		status = load_scene(argv[1], &data.scene);
	//En commentaire pour linstant car non implementer
	if (status == SUCCESS)
		status = run_app(&data);
	//test_provisoire :
	if (status == SUCCESS)
	 	print_scene(&data.scene);
	free_scene(&data.scene);
	return (status);
}
