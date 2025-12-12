/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_release.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 19:18:17 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/12 15:15:25 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "new_rt.h"
#include "display.h"

// int	mouse_release(int button, int x, int y, t_data *data)
// {
// 	(void)x;
// 	(void)y;
// 	if (button == 1 && data->display.ui.dragging)
// 	{
// 		data->display.ui.dragging = 0;
// 		data->display.flag_ui = 1;
// 	}
// 	return (0);
// }

int	mouse_release(int button, int x, int y, t_data *data)
{
	t_mouse_state	*mouse;

	(void)x;
	(void)y;
	mouse = &data->display.mouse;
	if ((button == 1 && mouse->button == MOUSE_BUTTON_LEFT)
		|| (button == 3 && mouse->button == MOUSE_BUTTON_RIGHT))
	{
		mouse->is_down = 0;
		mouse->button = MOUSE_BUTTON_NONE;
	}
	return (0);
}


