/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_app.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 12:48:46 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 07:39:11 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "new_rt.h"
#include "errors.h"
#include "mlx.h"
#include "logs.h"

void	init_surface_uv(t_scene *scene);
void	init_ui(t_display *display);
int		threads_start(t_data *data);
void	threads_cancel_and_join(t_data *data);
void	destroy_render(t_render *render);
void	destroy_display(t_display *display);

int	run_app(t_data *data)
{
	int	status;

	status = engine_init(&data->engine, &data->scene);
	if (status == SUCCESS)
		status = display_init(&data->display, data);
	if (status == SUCCESS)
		init_surface_uv(&data->scene);
	if (status == SUCCESS)
		init_ui(&data->display);
	if (status == SUCCESS)
		status = threads_start(data);
	if (status == SUCCESS)
		status = mlx_loop(data->display.mlx);
	threads_cancel_and_join(data);
	destroy_render(&data->engine.render);
	destroy_display(&data->display);
	return (status);
}
