#include <pthread.h>
#include "render.h"
#include "rt.h"
#include <stdatomic.h>
#include "new_rt.h"
#include <unistd.h>
#include "engine.h"

static void	wait_for_job(atomic_int *has_job, atomic_int *cancel_flag)
{
	while (atomic_load(has_job) == 0
		&& atomic_load(cancel_flag) == 0)
		usleep(100);
}

static void	wait_tile_consumed(atomic_int *is_done, atomic_int *cancel_flag)
{
	while (atomic_load(is_done) == 1
		&& atomic_load(cancel_flag) == 0)
		usleep(100);
}

void	*worker_thread(void *arg)
{
	t_worker	*worker;
	t_data		*data;
	t_render	*render;

	worker = (t_worker *)arg;
	data = worker->data;
	render = &data->engine.render;
	wait_for_job(&worker->has_job, &render->cancel_flag);
	while (atomic_load(&render->cancel_flag) == 0)
	{
		render_tile(data, &worker->tile, worker->tile.cam_view);
		atomic_store(&worker->tile.is_done, 1);
		wait_tile_consumed(&worker->tile.is_done, &render->cancel_flag);
		wait_for_job(&worker->has_job, &render->cancel_flag);
	}
	return (NULL);
}
