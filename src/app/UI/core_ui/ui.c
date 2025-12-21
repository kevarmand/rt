/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 09:13:13 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 09:14:15 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core.h"

static void	ui_commit_event(t_display *display)
{
	t_ui	*ui;

	ui = &display->ui;
	if (ui->event == -1)
		return ;
	ui_apply_event(ui, ui->event);
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
	display->flag_img_buffer = 1;
}

static void	ui_load_from_display(t_display *display)
{
	t_ui	*ui;

	ui = &display->ui;
	ui->render_mode_selected = display->user_render_mode;
	ui->ssaa_idx = display->render_ssaa;
	ui->tonemap_enabled = display->render_tonemap;
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
	ui_try_click_button(ui, mouse);
	ui_try_start_drag(ui, mouse);
	if (ui->dragging == 1)
		ui_drag_step(display, mouse);
	ui->mouse_was_down = mouse->is_down;
	ui_commit_event(display);
	ui_rebuild_if_needed(display);
	ui->was_visible = 1;
}
