#include <pthread.h>
#include "render.h"
#include <stdatomic.h>
#include "new_rt.h"
#include "errors.h"
#include <unistd.h>
#include "engine.h"
#include <stdlib.h>

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

#include <stdio.h>

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
		//le flag FAST_MODE NEXISTE PAS AU Utiliser le qulity dANS DISPLAY
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
	if (pthread_create(&data->engine.render.workers.array[worker_index].thread_id,
			NULL, worker_thread, args) != 0)
	{
		free(args);
		return (ERR_THREAD_CREATE);
	}
	return (SUCCESS);
}