/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_hook.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 19:18:09 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 08:01:43 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "new_rt.h"
#include "display.h"
#include <stdio.h>

void	mouse_next(int button, t_data *data, t_mouse_state *mouse)
{
	if (button == 2)
	{
		data->display.cam_ctrl.flag_level = 1;
		return ;
	}
	if (button == 4)
	{
		mouse->scroll_delta++;
		return ;
	}
	if (button == 5)
	{
		mouse->scroll_delta--;
		return ;
	}
}

int	mouse_press(int button, int x, int y, t_data *data)
{
	t_mouse_state	*mouse;

	mouse = &data->display.mouse;
	if (button == 1)
	{
		mouse->button = MOUSE_BUTTON_LEFT;
		mouse->is_down = 1;
		mouse->anchor_x = x;
		mouse->anchor_y = y;
		mouse->current_x = x;
		mouse->current_y = y;
		return (0);
	}
	if (button == 3)
	{
		mouse->button = MOUSE_BUTTON_RIGHT;
		mouse->is_down = 1;
		mouse->anchor_x = x;
		mouse->anchor_y = y;
		mouse->current_x = x;
		mouse->current_y = y;
		return (0);
	}
	mouse_next(button, data, mouse);
	return (0);
}
