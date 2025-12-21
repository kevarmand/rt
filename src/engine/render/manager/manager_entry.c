/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager_entry.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:26:01 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 01:50:59 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include "render.h"
#include "engine.h"
#include <stdatomic.h>
#include <unistd.h>
#include "engine.h"
#include "manager.h"
#include "errors.h"
#include "new_rt.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct s_manager_args
{
	t_mgr		*manager;
	t_data		*data;
	atomic_int	*cancel_flag;
}	t_manager_args;

static void	manager_idle(t_render *render)
{
	if (manager_read_mailbox(render))
		return ;
	usleep(200);
}

static void	manager_loop(t_render *render, atomic_int *cancel_flag,
				t_mgr *manager)
{
	int		has_work;

	while (atomic_load(cancel_flag) == 0)
	{
		has_work = 0;
		if (manager->render_in_progress == 0)
		{
			manager_idle(render);
			continue ;
		}
		while (1)
		{
			has_work |= manager_collect_tiles(render);
			has_work |= manager_assign_jobs(render);
			has_work |= manager_convert_hdr_to_rgb(render);
			if (!(manager->render_view.mode == QUALITY_FAST && manager
					->tileset.tiles_done != manager->tileset.tiles_total))
				break ;
		}
		has_work |= manager_run_tonemap(manager, render->width, render->height);
		has_work |= manager_update_display(render);
		has_work |= manager_read_mailbox(render);
		if (has_work == 0)
			usleep(100);
	}
}

void	*manager_entry(void *arg)
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
	free(arg);
	manager_loop(render, cancel_flag, manager);
	return (NULL);
}
