/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_manager.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 15:56:09 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/18 16:23:16 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include "render.h"
#include "engine.h"
#include "engine.h"
#include "errors.h"
#include "new_rt.h"
#include <stdlib.h>

typedef struct s_manager_args
{
	t_mgr		*manager;
	t_data		*data;
	atomic_int	*cancel_flag;
}	t_manager_args;

int	thread_start_manager(t_data *data)
{
	t_manager_args	*args;

	args = malloc(sizeof(t_manager_args));
	if (args == NULL)
		return (ERR_MALLOC);
	args->data = data;
	args->manager = &data->engine.render.manager;
	args->cancel_flag = &data->engine.render.cancel_flag;
	if (pthread_create(&data->engine.render.manager.thread_id,
			NULL, manager_entry, args) != 0)
	{
		free(args);
		return (ERR_THREAD_CREATE);
	}
	return (SUCCESS);
}
