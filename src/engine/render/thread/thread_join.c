/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_join.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 22:34:23 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 01:50:09 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <pthread.h>
#include "engine.h"
#include "errors.h"
#include "new_rt.h"

void	threads_cancel_and_join(t_data *data)
{
	int	index;

	atomic_store(&data->engine.render.cancel_flag, 1);
	index = 0;
	while (index < data->engine.render.workers_count)
	{
		pthread_join(data->engine.render.workers.array[index].thread_id,
			NULL);
		index++;
	}
	if (data->engine.render.manager_started)
		pthread_join(data->engine.render.manager.thread_id, NULL);
}
