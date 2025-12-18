/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   worker_thread.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 23:04:24 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/17 23:05:03 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include "render.h"
#include <stdatomic.h>
#include "new_rt.h"
#include "errors.h"
#include <unistd.h>
#include "engine.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct s_worker_args
{
	t_worker	*worker;
	t_data		*data;
	atomic_int	*cancel_flag;
}	t_worker_args;

static void	wait_for_job(atomic_int *worker_state, atomic_int *cancel_flag)
{
	while (atomic_load(worker_state) != WORKER_BUSY
		&& atomic_load(cancel_flag) == 0)
		usleep(100);
}

void	*worker_thread(void *arg)
{
	t_worker_args	*args;
	t_worker		*worker;
	t_data			*data;
	atomic_int		*cancel_flag;

	args = (t_worker_args *)arg;
	worker = args->worker;
	data = args->data;
	cancel_flag = args->cancel_flag;
	wait_for_job(&worker->worker_state, cancel_flag);
	while (atomic_load(cancel_flag) == 0)
	{
		if (worker->local_view.mode == QUALITY_NORMAL)
			render_tile_normal(data, &worker->tile, &worker->local_view);
		else if (worker->local_view.mode == QUALITY_FAST)
			render_tile_fast(data, &worker->tile, &worker->local_view);
		else if (worker->local_view.mode == QUALITY_SUPER)
			render_tile_super(data, &worker->tile, &worker->local_view);
		atomic_store(&worker->worker_state, WORKER_DONE);
		wait_for_job(&worker->worker_state, cancel_flag);
	}
	return (NULL);
}

int	worker_thread_start(t_data *data, int worker_index)
{
	t_worker_args	*args;

	args = malloc(sizeof(t_worker_args));
	if (args == NULL)
		return (ERR_MALLOC);
	args->worker = &data->engine.render.workers.array[worker_index];
	args->data = data;
	args->cancel_flag = &data->engine.render.cancel_flag;
	if (pthread_create(
			&data->engine.render.workers.array[worker_index].thread_id,
			NULL, worker_thread, args) != 0)
	{
		free(args);
		return (ERR_THREAD_CREATE);
	}
	return (SUCCESS);
}
