#include "render.h"
#include "rt.h"

#include <stdlib.h>
#include <string.h>
#include <pthread.h>

//gestion de pthread
/***
 * Create the threads for rendering
 * Each thread will render a tile
 * @param data: pointer to the t_data structure
 * @return SUCCESS or ERROR
 */
int	create_threads(t_data *data)
{
	int	err;
	int	i;

	if (NB_CORE == 1)
		return (SUCCESS);
	// Créer les threads
	i = 0;
	atomic_store(&data->render.cancel_flag, 0);
	atomic_store(&data->render.thread_next_id, 0);
	while (i < NB_CORE - 1)
	{
		err = pthread_create(&data->render.tiles[i].thread_id, NULL,
			(void *(*)(void *))worker_thread, (void *)data);
		if (err)
			return (ERROR);
		i++;
	}
	return (SUCCESS);
}
