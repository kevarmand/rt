/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_app.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 12:48:46 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/23 18:38:37 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "jesaispasquoi.h"

int	run_app(t_data *data)
{
	int status;
	
	status = engine_init(&data->engine, &data->scene);
	if (status == SUCCESS)
		status = display_init(&data->display, data->engine.width, data->engine.height);
	if (status == SUCCESS)
		set_up_flags(&data->engine);
	if (status == SUCCESS)
		status = render_loop(data);
	return (status);
}
