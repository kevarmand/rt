/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 09:14:19 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 09:15:15 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core.h"

int	ui_mouse_in_button(t_ui *ui, t_mouse_state *mouse, t_button *btn)
{
	int	x0;
	int	y0;

	x0 = ui->x + btn->x;
	y0 = ui->y + btn->y;
	if (mouse->current_x < x0 || mouse->current_x > x0 + btn->width)
		return (0);
	if (mouse->current_y < y0 || mouse->current_y > y0 + btn->height)
		return (0);
	return (1);
}

int	ui_is_clickable(t_button *btn)
{
	if (btn->label == UI_BTN_NONE)
		return (0);
	if (btn->label == UI_BTN_SSAA_VALUE)
		return (0);
	return (1);
}

int	ui_try_click_button(t_ui *ui, t_mouse_state *mouse)
{
	int	index;

	if (ui->dragging == 1 || ui->event != -1)
		return (0);
	if (!ui_mouse_just_pressed(ui, mouse))
		return (0);
	index = 0;
	while (index < ui->button_count)
	{
		if (ui_is_clickable(&ui->buttons[index])
			&& ui_mouse_in_button(ui, mouse, &ui->buttons[index]))
		{
			ui->event = ui->buttons[index].label;
			mouse->is_down = 0;
			mouse->button = MOUSE_BUTTON_NONE;
			return (1);
		}
		index++;
	}
	return (0);
}

void	ui_apply_event(t_ui *ui, int event_id)
{
	if (event_id == UI_BTN_CLOSE)
		ui->visible = 0;
	else if (event_id == UI_BTN_TONEMAP)
		ui->tonemap_enabled = !ui->tonemap_enabled;
	else if (event_id == UI_BTN_RENDER_AUTO)
		ui->render_mode_selected = USER_RENDER_AUTO;
	else if (event_id == UI_BTN_RENDER_FAST)
		ui->render_mode_selected = USER_RENDER_LOCK_FAST;
	else if (event_id == UI_BTN_RENDER_NORMAL)
		ui->render_mode_selected = USER_RENDER_LOCK_NORMAL;
	else if (event_id == UI_BTN_RENDER_SUPER)
		ui->render_mode_selected = USER_RENDER_LOCK_SUPER;
	else if (event_id == UI_BTN_SSAA_MINUS && ui->ssaa_idx > UI_SSAA_IDX_MIN)
		ui->ssaa_idx--;
	else if (event_id == UI_BTN_SSAA_PLUS && ui->ssaa_idx < UI_SSAA_IDX_MAX)
		ui->ssaa_idx++;
	else if (event_id == UI_BTN_MODE_LOCK)
		ui->mode_selected = UI_MODE_IDX_LOCK;
	else if (event_id == UI_BTN_MODE_ROLL)
		ui->mode_selected = UI_MODE_IDX_ROLL;
	else if (event_id == UI_BTN_MODE_FREE)
		ui->mode_selected = UI_MODE_IDX_FREE;
}

void	apply_event(t_display *display, t_ui *ui)
{
	int i;

	display->render_ssaa = ui->ssaa_idx;
	display->render_tonemap = ui->tonemap_enabled;
	display->flag_request_render = 1;
	display->cam_ctrl.mode = ui->mode_selected;
	display->user_render_mode = ui->render_mode_selected;
	display->ui.visible = 0;
	display->ui.was_visible = 0;
	i = 0;
	while (i < display->total_cams)
	{
		display->frame[i].quality = 0;
		i++;
	}
}
