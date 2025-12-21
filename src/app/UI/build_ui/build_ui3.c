/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ui3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 09:02:49 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 09:05:42 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "build_ui.h"

void	ui_colors_from_enabled(int enabled, int *color_bg, int *color_txt)
{
	*color_bg = UI_COLOR_INACTIVE;
	*color_txt = UI_COLOR_TEXT;
	if (enabled)
	{
		*color_bg = UI_COLOR_ACTIVE;
		*color_txt = UI_COLOR_TEXT_SELECTED;
	}
}

void	ui_add_close(t_ui *ui)
{
	t_button	btn;

	ui_btn_set_rect(&btn, UI_EXIT_X, UI_EXIT_Y, UI_EXIT_W);
	ui_btn_set_height(&btn, UI_EXIT_H);
	ui_btn_set_style(&btn, UI_BTN_CLOSE, UI_TXT_CLOSE, UI_COLOR_EXIT_BG);
	ui_btn_set_txt_color(&btn, UI_COLOR_TEXT);
	ui_push_button(ui, &btn);
}

void	ui_add_ssaa(t_ui *ui)
{
	t_button	btn;

	ui_btn_set_rect(&btn, UI_SSAA_MINUS_X, UI_SSAA_MINUS_Y, UI_SSAA_MINUS_W);
	ui_btn_set_height(&btn, UI_SSAA_MINUS_H);
	ui_btn_set_style(&btn, UI_BTN_SSAA_MINUS, UI_TXT_SSAA_MINUS,
		UI_COLOR_INACTIVE);
	ui_btn_set_txt_color(&btn, UI_COLOR_TEXT);
	ui_push_button(ui, &btn);
	ui_btn_set_rect(&btn, UI_SSAA_VALUE_X, UI_SSAA_VALUE_Y, UI_SSAA_VALUE_W);
	ui_btn_set_height(&btn, UI_SSAA_VALUE_H);
	ui_btn_set_style(&btn, UI_BTN_SSAA_VALUE, ui_ssaa_txt(ui->ssaa_idx),
		UI_COLOR_VALUE_BG);
	ui_btn_set_txt_color(&btn, UI_COLOR_VALUE_TEXT);
	ui_push_button(ui, &btn);
	ui_btn_set_rect(&btn, UI_SSAA_PLUS_X, UI_SSAA_PLUS_Y, UI_SSAA_PLUS_W);
	ui_btn_set_height(&btn, UI_SSAA_PLUS_H);
	ui_btn_set_style(&btn, UI_BTN_SSAA_PLUS, UI_TXT_SSAA_PLUS,
		UI_COLOR_INACTIVE);
	ui_btn_set_txt_color(&btn, UI_COLOR_TEXT);
	ui_push_button(ui, &btn);
}

void	ui_add_mode_lock(t_ui *ui)
{
	t_button	btn;
	int			color_bg;
	int			color_txt;

	ui_colors_from_selected(ui_is_mode_selected(ui, UI_MODE_IDX_LOCK),
		&color_bg, &color_txt);
	ui_btn_set_rect(&btn, UI_MODE_LOCK_X, UI_MODE_LOCK_Y, UI_MODE_LOCK_W);
	ui_btn_set_height(&btn, UI_MODE_LOCK_H);
	ui_btn_set_style(&btn, UI_BTN_MODE_LOCK, UI_TXT_LOCK, color_bg);
	ui_btn_set_txt_color(&btn, color_txt);
	ui_push_button(ui, &btn);
}

void	ui_add_mode_roll(t_ui *ui)
{
	t_button	btn;
	int			color_bg;
	int			color_txt;

	ui_colors_from_selected(ui_is_mode_selected(ui, UI_MODE_IDX_ROLL),
		&color_bg, &color_txt);
	ui_btn_set_rect(&btn, UI_MODE_ROLL_X, UI_MODE_ROLL_Y, UI_MODE_ROLL_W);
	ui_btn_set_height(&btn, UI_MODE_ROLL_H);
	ui_btn_set_style(&btn, UI_BTN_MODE_ROLL, UI_TXT_ROLL, color_bg);
	ui_btn_set_txt_color(&btn, color_txt);
	ui_push_button(ui, &btn);
}
