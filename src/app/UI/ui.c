#include "ui_int.h"
#include "ui_settings.h"
#include "display.h"
#include <stdio.h>

static int	ui_mouse_just_pressed(t_ui *ui, t_mouse_state *mouse)
{
	if (ui->mouse_was_down == 1)
		return (0);
	if (mouse->is_down == 0)
		return (0);
	if (mouse->button != MOUSE_BUTTON_LEFT)
		return (0);
	return (1);
}

static int	ui_mouse_in_header(t_ui *ui, t_mouse_state *mouse)
{
	if (mouse->current_x < ui->x || mouse->current_x > ui->x + ui->img.width)
		return (0);
	if (mouse->current_y < ui->y || mouse->current_y > ui->y + UI_HEADER_H)
		return (0);
	return (1);
}

static void	ui_try_start_drag(t_ui *ui, t_mouse_state *mouse)
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

static int	ui_clamp_int(int value, int min_v, int max_v)
{
	if (value < min_v)
		return (min_v);
	if (value > max_v)
		return (max_v);
	return (value);
}

static int	ui_drag_step(t_display *display, t_mouse_state *mouse)
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
	display->flag_ui = 1;
	return (1);
}

static int	ui_mouse_in_button(t_ui *ui, t_mouse_state *mouse, t_button *btn)
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

static int	ui_is_clickable(t_button *btn)
{
	if (btn->label == UI_BTN_NONE)
		return (0);
	if (btn->label == UI_BTN_SSAA_VALUE)
		return (0);
	return (1);
}

static int	ui_try_click_button(t_ui *ui, t_mouse_state *mouse)
{
	int	index;

	if (ui->dragging == 1 || ui->event != -1)
		return (0);
	if (!ui_mouse_just_pressed(ui, mouse))
		return (0);
	index = 0;
	while (index < ui->button_count)
	{
		printf("Checking button %d\n", ui->buttons[index].label);
		if (ui_is_clickable(&ui->buttons[index])
			&& ui_mouse_in_button(ui, mouse, &ui->buttons[index]))
		{
			printf("Button %d clicked\n", ui->buttons[index].label);
			ui->event = ui->buttons[index].label;
			mouse->is_down = 0;
			mouse->button = MOUSE_BUTTON_NONE;
			return (1);
		}
		index++;
	}
	return (0);
}

static void	ui_apply_event(t_ui *ui, int event_id)
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

static void	apply_event(t_display *display, t_ui *ui)
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
	display->flag_ui = 1;
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
	display->flag_ui = 1;
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
