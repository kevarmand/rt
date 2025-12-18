/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_app.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 12:48:46 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/18 16:16:10 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "new_rt.h"
#include "errors.h"
#include "mlx.h"
#include <stdio.h>
#include "logs.h"

void	init_surface_uv(t_scene *scene);

int	run_app(t_data *data)
{
	int	status;

	status = engine_init(&data->engine, &data->scene);
	if (status == SUCCESS)
		status = display_init(&data->display, data);
	if (status == SUCCESS)
		status = threads_start(data);
	if (status == SUCCESS)
		init_surface_uv(&data->scene);
	if (status == SUCCESS)
		status = mlx_loop(data->display.mlx);
	return (status);
}
