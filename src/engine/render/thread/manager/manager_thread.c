/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager_thread.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 22:04:44 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/23 22:24:30 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include "render.h"
#include "engine.h"
#include <stdatomic.h>
#include <unistd.h>
#include "engine.h"

void	*render_manager_thread(void *arg)
{
	t_data		*data;
	t_render	*render;

	data = (t_data *)arg;
	render = &data->engine.render;
	while (1)
	{
		if (atomic_load(&render->cancel_flag) != 0)
			break;
		if (atomic_load(&render->busy) == 0)
		{
			usleep(200);
			continue ;
		}
		if (atomic_load(&render->tiles_done) >= render->tiles_total)
		{
			manager_finish_frame(render);
			atomic_store(&render->busy, 0);
			continue ;
		}
		manager_collect_tiles(render);
		manager_assign_jobs(render);
		usleep(100);
	}
	return (NULL);
}
