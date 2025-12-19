/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_move.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 19:18:14 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/18 23:37:52 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "new_rt.h"
#include "display.h"

// int	mouse_move(int x, int y, t_data *data)
// {
// 	int new_x;
// 	int new_y;
// 	int max_x;
// 	int max_y;

// 	if (data->display.ui.dragging)
// 	{
// 		new_x = x - data->display.ui.drag_offset_x;
// 		new_y = y - data->display.ui.drag_offset_y;
// 		max_x = data->engine.width - data->display.ui.img.width;
// 		max_y = data->engine.height - data->display.ui.img.height;
// 		if (new_x < 0)
// 			new_x = 0;
// 		else if (new_x > max_x)
// 			new_x = max_x;
// 		if (new_y < 0)
// 			new_y = 0;
// 		else if (new_y > max_y)
// 			new_y = max_y;
// 		data->display.ui.x = new_x;
// 		data->display.ui.y = new_y;
// 		data->display.flag_ui = 1;
// 	}
// 	return (0);
// }

int	mouse_move(int x, int y, t_data *data)
{
	t_mouse_state	*mouse;

	mouse = &data->display.mouse;
	mouse->current_x = x;
	mouse->current_y = y;
	return (0);
}
