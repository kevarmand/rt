/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine_tick.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 20:26:48 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/23 20:27:04 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"

int	render_swap_buffers(t_render *render)
{
	int	ready;
	int	busy;
	int	*tmp;

	ready = atomic_load(&render->rgb_ready);
	if (!ready)
		return (0);
	busy = atomic_load(&render->rgb_busy);
	if (busy)
		return (0);
	atomic_store(&render->rgb_busy, 1);
	tmp = render->rgb_front;
	render->rgb_front = render->rgb_back;
	render->rgb_back = tmp;
	atomic_store(&render->rgb_ready, 0);
	atomic_store(&render->rgb_busy, 0);
	return (1);
}

int	engine_tick(t_engine *engine)
{
	int	image_modified;

	image_modified = render_swap_buffers(&engine->render);
	return (image_modified);
}
