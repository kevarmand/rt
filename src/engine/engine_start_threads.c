/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine_start_threads.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 20:39:18 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/28 10:59:08 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <pthread.h>
#include "engine.h"
#include "errors.h"
#include "new_rt.h"



int		manager_thread_start(t_data *data);
int		worker_thread_start(t_data *data, int worker_index);


static int	start_manager_thread(t_data *data)
{
	int	status;

	status = manager_thread_start(data);
	if (status != 0)
		return (ERR_THREAD_CREATE);
	return (SUCCESS);
}

static int	start_worker_threads(t_data *data)
{
	int	index;
	int	status;

	index = 0;
	while (index < data->engine.render.workers.count)
	{
		status = worker_thread_start(data, index);
		if (status != 0)
			return (ERR_THREAD_CREATE);
		index++;
	}
	return (SUCCESS);
}

int	engine_start_threads(t_data *data)
{
	int	status;

	status = start_manager_thread(data);
	if (status != SUCCESS)
		return (status);
	status = start_worker_threads(data);
	return (status);
}
