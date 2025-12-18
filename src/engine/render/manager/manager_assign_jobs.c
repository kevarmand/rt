/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager_assign_jobs.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 15:29:07 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/17 23:25:23 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "manager.h"
#include <stdatomic.h>
#include "render.h"

static int	dispatch_next_tile(t_render *render, t_worker *worker)
{
	t_mgr	*mgr;

	mgr = &render->manager;
	if (init_worker_tile(&mgr->tileset, &worker->tile,
			render->width, render->height) == 0)
		return (0);
	worker->local_view = mgr->render_view;
	mgr->tileset.tiles_active++;
	atomic_store(&worker->worker_state, WORKER_BUSY);
	return (1);
}

int	manager_assign_jobs(t_render *render)
{
	t_workers	*workers;
	int			worker_index;
	int			did_work;
	t_worker	*worker;

	workers = &render->workers;
	worker_index = 0;
	did_work = 0;
	while (worker_index < workers->count)
	{
		worker = &workers->array[worker_index];
		if (atomic_load(&worker->worker_state) == WORKER_IDLE)
			did_work |= dispatch_next_tile(render, worker);
		worker_index++;
	}
	return (did_work);
}
