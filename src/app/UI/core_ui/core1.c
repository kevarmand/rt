/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 09:13:29 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 09:13:53 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core.h"

int	ui_mouse_just_pressed(t_ui *ui, t_mouse_state *mouse)
{
	if (ui->mouse_was_down == 1)
		return (0);
	if (mouse->is_down == 0)
		return (0);
	if (mouse->button != MOUSE_BUTTON_LEFT)
		return (0);
	return (1);
}

int	ui_mouse_in_header(t_ui *ui, t_mouse_state *mouse)
{
	if (mouse->current_x < ui->x || mouse->current_x > ui->x + ui->img.width)
		return (0);
	if (mouse->current_y < ui->y || mouse->current_y > ui->y + UI_HEADER_H)
		return (0);
	return (1);
}

void	ui_try_start_drag(t_ui *ui, t_mouse_state *mouse)
{
	if (ui->dragging == 1)
		return ;
	if (!ui_mouse_just_pressed(ui, mouse))
		return ;
	if (!ui_mouse_in_header(ui, mouse))
		return ;
	ui->dragging = 1;
	mouse->anchor_x = mouse->current_x;
	mouse->anchor_y = mouse->current_y;
}

int	ui_clamp_int(int value, int min_v, int max_v)
{
	if (value < min_v)
		return (min_v);
	if (value > max_v)
		return (max_v);
	return (value);
}

int	ui_drag_step(t_display *display, t_mouse_state *mouse)
{
	int	new_x;
	int	new_y;
	int	max_x;
	int	max_y;

	new_x = display->ui.x + mouse->current_x - mouse->anchor_x;
	new_y = display->ui.y + mouse->current_y - mouse->anchor_y;
	mouse->anchor_x = mouse->current_x;
	mouse->anchor_y = mouse->current_y;
	max_x = display->main_img.width - display->ui.img.width;
	max_y = display->main_img.height - display->ui.img.height;
	display->ui.x = ui_clamp_int(new_x, 0, max_x);
	display->ui.y = ui_clamp_int(new_y, 0, max_y);
	if (mouse->is_down == 0)
		display->ui.dragging = 0;
	display->flag_img_buffer = 1;
	return (1);
}
