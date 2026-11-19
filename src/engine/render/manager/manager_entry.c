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

#include "manager.h"
#include "new_rt.h"
#include <stdlib.h>

typedef struct s_manager_args
{
	t_mgr		*manager;
	t_data		*data;
	atomic_int	*cancel_flag;
}	t_manager_args;

static void	manager_loop(t_render *render, atomic_int *cancel_flag,
				t_mgr *manager)
{
	t_hdr_frame	*active;
	int			has_work;

	while (atomic_load(cancel_flag) == 0)
	{
		manager->post_settings = atomic_load(&render->mailbox.post_settings);
		manager->active_cam = atomic_load(&render->mailbox.active_cam);
		if (manager->render_in_progress && manager->render_view.frame_seq
			!= atomic_load(&render->mailbox.expected_seq[manager->current_cam_id]))
			manager->render_in_progress = 0;
		has_work = manager_read_mailbox(render);
		/* Give active slider feedback the spare before collecting more HDR. */
		active = &manager->frames[manager->active_cam];
		if (active->published_settings
			&& active->published_settings != manager->post_settings
			&& manager_update_display(render))
			continue ;
		has_work |= manager_collect_tiles(render);
		if (manager->render_in_progress
			&& manager->frames[manager->current_cam_id].done
			== manager->tileset.tiles_total)
			manager_finish_render(render);
		if (manager->render_in_progress)
			has_work |= manager_assign_jobs(render);
		has_work |= manager_update_display(render);
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
