#include <pthread.h>
#include "render.h"
#include "rt.h"
#include <stdatomic.h>
#include "new_rt.h"
#include <unistd.h>
#include "engine.h"

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
		render_tile(data, &worker->tile, &worker->local_view);
		atomic_store(&worker->worker_state, WORKER_IDLE);
		wait_for_job(&worker->worker_state, cancel_flag);
	}
	return (NULL);
}
