/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   worker_entry.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 15:52:53 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/18 16:26:42 by kearmand         ###   ########.fr       */
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

void	*worker_entry(void *arg)
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
