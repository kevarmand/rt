/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ui.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 15:13:29 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/19 16:46:11 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ui_int.h"
#include "ui_settings.h"

static const char	*ui_ssaa_txt(int idx)
{
	if (idx == UI_SSAA_IDX_MIN)
		return ("1");
	if (idx == UI_SSAA_IDX_MIN + 1)
		return ("4");
	if (idx == UI_SSAA_IDX_MIN + 2)
		return ("9");
	if (idx == UI_SSAA_IDX_MIN + 3)
		return ("16");
	return ("25");
}

static const char	*ui_onoff_txt(int enabled)
{
	if (enabled)
		return (UI_TXT_ON);
	return (UI_TXT_OFF);
}

static int	ui_is_mode_selected(t_ui *ui, int mode_idx)
{
	if (ui->mode_selected == mode_idx)
		return (1);
	return (0);
}

static void	ui_push_button(t_ui *ui, t_button *btn)
{
	ui->buttons[ui->button_count] = *btn;
	ui->button_count++;
}

static void	ui_add_close(t_ui *ui)
{
	t_button	btn;

	btn.x = UI_EXIT_X;
	btn.y = UI_EXIT_Y;
	btn.width = UI_EXIT_W;
	btn.height = UI_EXIT_H;
	btn.label = UI_BTN_CLOSE;
	btn.txt = UI_TXT_CLOSE;
	btn.color_bg = UI_COLOR_EXIT_BG;
	btn.color_txt = UI_COLOR_TEXT;
	ui_push_button(ui, &btn);
}

static void	ui_add_ssaa(t_ui *ui)
{
	t_button	btn;

	btn.x = UI_SSAA_MINUS_X;
	btn.y = UI_SSAA_MINUS_Y;
	btn.width = UI_SSAA_MINUS_W;
	btn.height = UI_SSAA_MINUS_H;
	btn.label = UI_BTN_SSAA_MINUS;
	btn.txt = UI_TXT_SSAA_MINUS;
	btn.color_bg = UI_COLOR_INACTIVE;
	btn.color_txt = UI_COLOR_TEXT;
	ui_push_button(ui, &btn);
	btn.x = UI_SSAA_VALUE_X;
	btn.y = UI_SSAA_VALUE_Y;
	btn.width = UI_SSAA_VALUE_W;
	btn.height = UI_SSAA_VALUE_H;
	btn.label = UI_BTN_SSAA_VALUE;
	btn.txt = ui_ssaa_txt(ui->ssaa_idx);
	btn.color_bg = UI_COLOR_VALUE_BG;
	btn.color_txt = UI_COLOR_VALUE_TEXT;
	ui_push_button(ui, &btn);
	btn.x = UI_SSAA_PLUS_X;
	btn.y = UI_SSAA_PLUS_Y;
	btn.width = UI_SSAA_PLUS_W;
	btn.height = UI_SSAA_PLUS_H;
	btn.label = UI_BTN_SSAA_PLUS;
	btn.txt = UI_TXT_SSAA_PLUS;
	btn.color_bg = UI_COLOR_INACTIVE;
	btn.color_txt = UI_COLOR_TEXT;
	ui_push_button(ui, &btn);
}

static void	ui_add_mode_lock(t_ui *ui)
{
	t_button	btn;

	btn.x = UI_MODE_LOCK_X;
	btn.y = UI_MODE_LOCK_Y;
	btn.width = UI_MODE_LOCK_W;
	btn.height = UI_MODE_LOCK_H;
	btn.label = UI_BTN_MODE_LOCK;
	btn.txt = UI_TXT_LOCK;
	btn.color_bg = ui_is_mode_selected(ui, UI_MODE_IDX_LOCK)
		? UI_COLOR_ACTIVE : UI_COLOR_INACTIVE;
	btn.color_txt = ui_is_mode_selected(ui, UI_MODE_IDX_LOCK)
		? UI_COLOR_TEXT_SELECTED : UI_COLOR_TEXT;
	ui_push_button(ui, &btn);
}

static void	ui_add_mode_roll(t_ui *ui)
{
	t_button	btn;

	btn.x = UI_MODE_ROLL_X;
	btn.y = UI_MODE_ROLL_Y;
	btn.width = UI_MODE_ROLL_W;
	btn.height = UI_MODE_ROLL_H;
	btn.label = UI_BTN_MODE_ROLL;
	btn.txt = UI_TXT_ROLL;
	btn.color_bg = ui_is_mode_selected(ui, UI_MODE_IDX_ROLL)
		? UI_COLOR_ACTIVE : UI_COLOR_INACTIVE;
	btn.color_txt = ui_is_mode_selected(ui, UI_MODE_IDX_ROLL)
		? UI_COLOR_TEXT_SELECTED : UI_COLOR_TEXT;
	ui_push_button(ui, &btn);
}

static void	ui_add_mode_free(t_ui *ui)
{
	t_button	btn;

	btn.x = UI_MODE_FREE_X;
	btn.y = UI_MODE_FREE_Y;
	btn.width = UI_MODE_FREE_W;
	btn.height = UI_MODE_FREE_H;
	btn.label = UI_BTN_MODE_FREE;
	btn.txt = UI_TXT_FREE;
	btn.color_bg = ui_is_mode_selected(ui, UI_MODE_IDX_FREE)
		? UI_COLOR_ACTIVE : UI_COLOR_INACTIVE;
	btn.color_txt = ui_is_mode_selected(ui, UI_MODE_IDX_FREE)
		? UI_COLOR_TEXT_SELECTED : UI_COLOR_TEXT;
	ui_push_button(ui, &btn);
}

static void	ui_add_tonemap_label(t_ui *ui)
{
	t_button	btn;

	btn.x = UI_TONEMAP_LABEL_X;
	btn.y = UI_TONEMAP_LABEL_Y;
	btn.width = UI_TONEMAP_LABEL_W;
	btn.height = UI_TONEMAP_LABEL_H;
	btn.label = UI_BTN_NONE;
	btn.txt = UI_TXT_TONEMAP;
	btn.color_bg = UI_COLOR_INACTIVE;
	btn.color_txt = UI_COLOR_TEXT;
	ui_push_button(ui, &btn);
}

static void	ui_add_tonemap_toggle(t_ui *ui)
{
	t_button	btn;

	btn.x = UI_TONEMAP_TOGGLE_X;
	btn.y = UI_TONEMAP_TOGGLE_Y;
	btn.width = UI_TONEMAP_TOGGLE_W;
	btn.height = UI_TONEMAP_TOGGLE_H;
	btn.label = UI_BTN_TONEMAP;
	btn.txt = ui_onoff_txt(ui->tonemap_enabled);
	btn.color_bg = ui->tonemap_enabled ? UI_COLOR_ACTIVE : UI_COLOR_INACTIVE;
	btn.color_txt = ui->tonemap_enabled
		? UI_COLOR_TEXT_SELECTED : UI_COLOR_TEXT;
	ui_push_button(ui, &btn);
}
static void	ui_add_apply(t_ui *ui)
{
	t_button	btn;

	btn.x = UI_APPLY_X;
	btn.y = UI_APPLY_Y;
	btn.width = UI_APPLY_W;
	btn.height = UI_APPLY_H;
	btn.label = UI_BTN_APPLY;
	btn.txt = UI_TXT_APPLY;
	btn.color_bg = UI_COLOR_ACTIVE;
	btn.color_txt = UI_COLOR_VALUE_TEXT;
	ui_push_button(ui, &btn);
}

void	ui_build_buttons(t_ui *ui)
{
	ui->button_count = 0;
	ui_add_close(ui);
	ui_add_tonemap_label(ui);
	ui_add_tonemap_toggle(ui);
	ui_add_ssaa(ui);
	ui_add_mode_lock(ui);
	ui_add_mode_roll(ui);
	ui_add_mode_free(ui);
	ui_add_apply(ui);
}
