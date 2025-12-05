/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_move.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 19:18:14 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/05 16:07:39 by kearmand         ###   ########.fr       */
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
	int				delta_x;
	int				delta_y;

	mouse = &data->display.mouse;
	if (mouse->mode == 0)
		return (0);
	delta_x = x - mouse->last_x;
	delta_y = y - mouse->last_y;
	mouse->last_x = x;
	mouse->last_y = y;
	mouse->accum_dx += delta_x;
	mouse->accum_dy += delta_y;
	/* Si plus tard mode == 1 (UI), ici tu appelleras ui_mouse_move */
	return (0);
}
