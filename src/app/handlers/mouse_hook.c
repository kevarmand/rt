/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_hook.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 19:18:09 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/06 15:10:26 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "new_rt.h"
#include "display.h"

// int	mouse_drag_ui(int rel_x, int rel_y, t_data *data)
// {
// 	data->display.ui.dragging = 1;
// 	data->display.ui.drag_offset_x = rel_x;
// 	data->display.ui.drag_offset_y = rel_y;
// 	return (1);
// }

// int	mouse_click_ui(int rel_x, int rel_y, t_data *data)
// {
// 	t_ui		*ui = &(data->display.ui);
// 	t_button	*btn;
// 	int			i;

// 	i = ui->button_count;
// 	while (--i >= 0)
// 	{
// 		btn = &ui->buttons[i];
// 		if (rel_x >= btn->x && rel_x < btn->x + btn->width &&
// 			rel_y >= btn->y && rel_y < btn->y + btn->height)
// 		{
// 			if  (i  == 0)
// 				return (mouse_drag_ui(rel_x, rel_y, data));
// 			if (btn->on_click)
// 			{
// 				btn->on_click(data);
// 				printf("Button[%i] clicked: %s\n", i, btn->txt ? btn->txt : "No text");
// 				return (1); // bouton trouvé et action exécutée
// 			}
// 		}

// 	}
// 	return (0); // aucun bouton cliqué
// }

// int	mouse_press(int button, int x, int y, t_data *data)
// {
// 	int	rel_x;
// 	int	rel_y;
// 	if (button == 1 && data->display.ui.visible)
// 	{
// 		rel_x = x - data->display.ui.x;
// 		rel_y = y - data->display.ui.y;
// 		if (mouse_click_ui(rel_x, rel_y, data))
// 			data->display.flag_ui = 1;
// 	}
// 	return (0);
// }

#include <stdio.h>

int	mouse_press(int button, int x, int y, t_data *data)
{
	t_mouse_state	*mouse;

	mouse = &data->display.mouse;
	if (button == 1)
	{
		mouse->mode = MOUSE_MODE_CAM_FREE;
		mouse->is_down = 1;
		mouse->anchor_x = x;
		mouse->anchor_y = y;
		mouse->current_x = x;
		mouse->current_y = y;
		return (0);
	}
	if (button == 3)
	{
		mouse->mode = MOUSE_MODE_CAM_ROLL;
		mouse->is_down = 1;
		mouse->anchor_x = x;
		mouse->anchor_y = y;
		mouse->current_x = x;
		mouse->current_y = y;
		mouse->roll_prev_angle = 0.0f;
		return (0);
	}
	if (button == 2)
	{
		data->display.flag_camera_level = 1;
		printf("Resetting camera level.\n");
		return (0);
	}
	if (button == 4)
	{
		data->display.mouse.scroll_delta++;
		return (0);
	}
	if (button == 5)
	{
		data->display.mouse.scroll_delta--;
		return (0);
	}
	return (0);
}
