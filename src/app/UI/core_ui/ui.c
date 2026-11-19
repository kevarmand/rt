/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 09:13:13 by kearmand          #+#    #+#             */
/*   Updated: 2026/11/12 14:32:33 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core.h"

static void	ui_apply_post_action(t_display *display, t_ui *ui)
{
	if (ui->event == UI_BTN_AUTO_EXPOSURE
		&& display->frame[display->current_cam].state == FRAME_READY)
		display->post_ev_step = display->frame[display->current_cam].auto_ev;
	else if (ui->event == UI_BTN_POST_RESET)
	{
		display->post_ev_step = 0;
		display->post_gamma_step = POST_GAMMA_DEFAULT;
	}
	ui->ev_step = display->post_ev_step;
	ui->gamma_step = display->post_gamma_step;
}

static void	ui_update_auto_available(t_display *display, t_ui *ui)
{
	int	available;

	available = (display->frame[display->current_cam].state == FRAME_READY);
	if (ui->auto_available != available)
	{
		ui->auto_available = available;
		ui->dirty = 1;
	}
}

static void	ui_commit_event(t_display *display)
{
	t_ui	*ui;

	ui = &display->ui;
	if (ui->event == -1)
		return ;
	ui_apply_event(ui, ui->event);
	ui_apply_post_action(display, ui);
	if (ui->event == UI_BTN_APPLY)
		apply_event(display, ui);
	ui->event = -1;
	ui->dirty = 1;
}

static void	ui_rebuild_if_needed(t_display *display)
{
	t_ui	*ui;

	ui = &display->ui;
	if (ui->dirty == 0)
		return ;
	ui_build_buttons(ui);
	draw_ui(ui);
	ui->dirty = 0;
	display->flag_img_window = 1;
}

static void	ui_load_from_display(t_display *display)
{
	t_ui	*ui;

	ui = &display->ui;
	ui->render_mode_selected = display->user_render_mode;
	ui->ssaa_idx = display->render_ssaa - 1;
	ui->gamma_step = display->post_gamma_step;
	ui->ev_step = display->post_ev_step;
	ui->slider_drag = 0;
	ui->mode_selected = display->cam_ctrl.mode;
	ui->dirty = 1;
}

static void	ui_on_open_if_needed(t_display *display)
{
	t_ui	*ui;

	ui = &display->ui;
	if (ui->visible == 0)
		return ;
	if (ui->was_visible == 1)
		return ;
	ui_load_from_display(display);
}

static int	ui_update_slider(t_display *display, t_ui *ui,
		t_mouse_state *mouse)
{
	int	offset;
	int	value;
	int	y;

	if (!mouse->is_down || mouse->button != MOUSE_BUTTON_LEFT)
	{
		ui->slider_drag = 0;
		return (0);
	}
	if (!ui->slider_drag && !ui->dragging && ui_mouse_just_pressed(ui, mouse)
		&& mouse->current_x >= ui->x + UI_SLIDER_X
		&& mouse->current_x <= ui->x + UI_SLIDER_X + UI_SLIDER_W)
	{
		y = mouse->current_y - ui->y;
		if (y >= UI_GAMMA_Y && y < UI_GAMMA_Y + UI_SLIDER_H)
			ui->slider_drag = UI_BTN_GAMMA;
		else if (y >= UI_EXPOSURE_Y && y < UI_EXPOSURE_Y + UI_SLIDER_H)
			ui->slider_drag = UI_BTN_EXPOSURE;
	}
	if (!ui->slider_drag)
		return (0);
	offset = ui_clamp_int(mouse->current_x - ui->x - UI_SLIDER_X,
			0, UI_SLIDER_W);
	if (ui->slider_drag == UI_BTN_GAMMA)
	{
		value = POST_GAMMA_MIN + (offset * (POST_GAMMA_MAX - POST_GAMMA_MIN)
				+ UI_SLIDER_W / 2) / UI_SLIDER_W;
		if (value != ui->gamma_step)
			ui->dirty = 1;
		ui->gamma_step = value;
		display->post_gamma_step = value;
	}
	else
	{
		value = POST_EV_MIN + (offset * (POST_EV_MAX - POST_EV_MIN)
				+ UI_SLIDER_W / 2) / UI_SLIDER_W;
		if (value != ui->ev_step)
			ui->dirty = 1;
		ui->ev_step = value;
		display->post_ev_step = value;
	}
	return (1);
}

void	display_update_ui(t_display *display)
{
	t_ui			*ui;
	t_mouse_state	*mouse;

	ui = &display->ui;
	mouse = &display->mouse;
	if (ui->visible == 0)
	{
		ui->was_visible = 0;
		return ;
	}
	ui_on_open_if_needed(display);
	ui_update_auto_available(display, ui);
	if (!ui_update_slider(display, ui, mouse))
	{
		ui_try_click_button(ui, mouse);
		ui_try_start_drag(ui, mouse);
	}
	if (ui->dragging == 1)
		ui_drag_step(display, mouse);
	ui->mouse_was_down = mouse->is_down;
	ui_commit_event(display);
	ui_rebuild_if_needed(display);
	ui->was_visible = 1;
}
