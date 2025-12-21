/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_start.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:18:48 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 07:30:18 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include "engine.h"
#include "errors.h"
#include "new_rt.h"

int		thread_start_manager(t_data *data);
int		thread_start_worker(t_data *data, int worker_index);

static int	start_manager_thread(t_data *data)
{
	int	status;

	status = thread_start_manager(data);
	data->engine.render.manager_started = !status;
	if (status != 0)
		return (ERR_THREAD);
	return (SUCCESS);
}

static int	start_worker_threads(t_data *data)
{
	int	index;
	int	status;

	index = 0;
	data->engine.render.workers_count = 0;
	while (index < data->engine.render.workers.count)
	{
		status = thread_start_worker(data, index);
		if (status != 0)
			return (ERR_THREAD);
		(data->engine.render.workers_count)++;
		index++;
	}
	return (SUCCESS);
}

int	threads_start(t_data *data)
{
	int	status;

	status = start_manager_thread(data);
	if (status != SUCCESS)
		return (status);
	status = start_worker_threads(data);
	return (status);
}
