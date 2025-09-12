#include <pthread.h>
#include "render.h"
#include "rt.h"
#include <stdatomic.h>



void	wait_work(atomic_int *tile_done, atomic_int *cancel)
{
	while (atomic_load(tile_done) == 1 && atomic_load(cancel) == 0)
		usleep(999);//Parce au on adore les magic number
}

void *worker_thread(void *arg)
{
	static _Atomic int count_loop = 0;
	t_data	*data;
	t_tile	*tile;
	int		thread_id;

	data = (t_data *)arg;
	thread_id = atomic_fetch_add(&data->render.thread_next_id, 1);
	tile = &data->render.tiles[thread_id];
	//debug on verifie que le trhead est lancer printf
	wait_work(&tile->is_done, &data->render.cancel_flag);
	while (atomic_load(&data->render.cancel_flag) == 0)
	{
		render_tile(data, tile, data->render.camera);
		atomic_store(&tile->is_done, 1);
		wait_work(&tile->is_done, &data->render.cancel_flag);
		printf("thread %d done tile %d/%d\n", thread_id, ++count_loop, data->render.tiles_total);
	}
	printf("thread %lu exiting\n", tile->thread_id);
	return (NULL);
}