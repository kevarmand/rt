/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_app.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 12:48:46 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/28 10:59:36 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "new_rt.h"
#include "errors.h"
#include "mlx.h"

#include <stdio.h>
int	run_app(t_data *data)
{
	int status;
	
	status = engine_init(&data->engine, &data->scene);
		printf("Application started successfully.\n");
	printf(" status = %d\n", status);
	if (status == SUCCESS)
		status = display_init(&data->display, data);
			printf("Application started successfully.\n");
	printf(" status = %d\n", status);
	if (status == SUCCESS)
		status = engine_start_threads(data);
	printf("Application started successfully.\n");
	printf(" status = %d\n", status);
	if (status == SUCCESS)
		status = mlx_loop(data->display.mlx);
	return (status);
}
