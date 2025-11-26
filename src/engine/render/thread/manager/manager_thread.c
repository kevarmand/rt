/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager_thread.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 22:04:44 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/26 18:38:31 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include "render.h"
#include "engine.h"
#include <stdatomic.h>
#include <unistd.h>
#include "engine.h"
#include "manager.h"

#include "new_rt.h"
//je commence comme toi :

typedef struct s_manager_args
{
	t_mgr		*manager;
	t_data		*data;
	atomic_int	*cancel_flag;
}	t_manager_args;

static void	manager_idle(t_data *data, t_render *render)
{
	if (manager_apply_cam_request(data, render))
		return ;
	usleep(200);
}

static void manager_loop(t_render *render, t_data *data, atomic_int *cancel_flag,t_mgr *manager)
{
	int		has_work;

	has_work = 0;
	while (atomic_load(cancel_flag) == 0)
	{
		if(manager->render_in_progress == 0)
		{
			manager_idle(data, render);
			continue ;
		}
		has_work |= manager_collect_tiles(render);
		has_work |= manager_assign_jobs(render);
		if (render->manager.tileset.tiles_done 
			== render->manager.tileset.tiles_total)
			manager_run_tonemap(render);
		else
			has_work |= manager_convert_hdr_to_rgb(render);
		has_work |= manager_update_display(render);
		has_work |= manager_read_mailbox(render);
		if (has_work == 0)
			usleep(100);
	}
}

void	*manager_thread_main(void *arg)
{
	t_manager_args	*args;
	t_data			*data;
	t_render		*render;
	t_mgr			*manager;
	atomic_int		*cancel_flag;

	args = (t_manager_args *)arg;
	data = args->data;
	render = &data->engine.render;
	manager = args->manager;
	cancel_flag = args->cancel_flag;
	manager_loop(render, data, cancel_flag, manager);
	return (NULL);
}

