/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_worker.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:18:36 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 07:23:41 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include "render.h"
#include <stdatomic.h>
#include "new_rt.h"
#include "errors.h"
#include "engine.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct s_worker_args
{
	t_worker	*worker;
	t_data		*data;
	atomic_int	*cancel_flag;
}	t_worker_args;

int	thread_start_worker(t_data *data, int worker_index)
{
	t_worker_args	*args;

	args = malloc(sizeof(t_worker_args));
	if (args == NULL)
		return (perr(ERR_MALLOC, PERR_M_THREAD_ARGS));
	args->worker = &data->engine.render.workers.array[worker_index];
	args->data = data;
	args->cancel_flag = &data->engine.render.cancel_flag;
	if (pthread_create(
			&data->engine.render.workers.array[worker_index].thread_id,
			NULL, worker_entry, args) != 0)
	{
		free(args);
		return (perr(ERR_THREAD, PERR_M_THREAD_CREATE));
	}
	return (SUCCESS);
}
