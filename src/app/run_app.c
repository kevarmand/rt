/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_app.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 12:48:46 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/01 17:55:45 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "new_rt.h"
#include "errors.h"
#include "mlx.h"

void	init_surface_uv(t_scene *scene);

#include <stdio.h>
int	run_app(t_data *data)
{
	int status;
	
	status = engine_init(&data->engine, &data->scene);
	printf("Engine init successfully.\n");
	printf(" status = %d\n", status);
	if (status == SUCCESS)
		status = display_init(&data->display, data);
	printf("Display init successfully.\n");
	printf(" status = %d\n", status);
	
	if (status == SUCCESS)
		status = engine_start_threads(data);
	printf("Engine threads started successfully.\n");

	if (status == SUCCESS)
		init_surface_uv(&data->scene);
	printf("Surface UV initialized successfully.\n");
	printf(" status = %d\n", status);
	if (status == SUCCESS)
		status = mlx_loop(data->display.mlx);
	return (status);
}
