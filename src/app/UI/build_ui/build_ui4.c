/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ui4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 09:03:24 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 09:03:32 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "build_ui.h"

void	ui_add_mode_free(t_ui *ui)
{
	t_button	btn;
	int			color_bg;
	int			color_txt;

	ui_colors_from_selected(ui_is_mode_selected(ui, UI_MODE_IDX_FREE),
		&color_bg, &color_txt);
	ui_btn_set_rect(&btn, UI_MODE_FREE_X, UI_MODE_FREE_Y, UI_MODE_FREE_W);
	ui_btn_set_height(&btn, UI_MODE_FREE_H);
	ui_btn_set_style(&btn, UI_BTN_MODE_FREE, UI_TXT_FREE, color_bg);
	ui_btn_set_txt_color(&btn, color_txt);
	ui_push_button(ui, &btn);
}

void	ui_add_tonemap_label(t_ui *ui)
{
	t_button	btn;

	ui_btn_set_rect(&btn, UI_TONEMAP_LABEL_X, UI_TONEMAP_LABEL_Y,
		UI_TONEMAP_LABEL_W);
	ui_btn_set_height(&btn, UI_TONEMAP_LABEL_H);
	ui_btn_set_style(&btn, UI_BTN_NONE, UI_TXT_TONEMAP, UI_COLOR_INACTIVE);
	ui_btn_set_txt_color(&btn, UI_COLOR_TEXT);
	ui_push_button(ui, &btn);
}

void	ui_add_tonemap_toggle(t_ui *ui)
{
	t_button	btn;
	int			color_bg;
	int			color_txt;

	ui_colors_from_enabled(ui->tonemap_enabled, &color_bg, &color_txt);
	ui_btn_set_rect(&btn, UI_TONEMAP_TOGGLE_X, UI_TONEMAP_TOGGLE_Y,
		UI_TONEMAP_TOGGLE_W);
	ui_btn_set_height(&btn, UI_TONEMAP_TOGGLE_H);
	ui_btn_set_style(&btn, UI_BTN_TONEMAP, ui_onoff_txt(ui->tonemap_enabled),
		color_bg);
	ui_btn_set_txt_color(&btn, color_txt);
	ui_push_button(ui, &btn);
}

void	ui_add_apply(t_ui *ui)
{
	t_button	btn;

	ui_btn_set_rect(&btn, UI_APPLY_X, UI_APPLY_Y, UI_APPLY_W);
	ui_btn_set_height(&btn, UI_APPLY_H);
	ui_btn_set_style(&btn, UI_BTN_APPLY, UI_TXT_APPLY, UI_COLOR_ACTIVE);
	ui_btn_set_txt_color(&btn, UI_COLOR_VALUE_TEXT);
	ui_push_button(ui, &btn);
}

void	ui_add_render_auto(t_ui *ui)
{
	t_button	btn;
	int			color_bg;
	int			color_txt;

	ui_colors_from_selected(ui_is_render_selected(ui, USER_RENDER_AUTO),
		&color_bg, &color_txt);
	ui_btn_set_rect(&btn, UI_RENDER_AUTO_X, UI_RENDER_AUTO_Y, UI_RENDER_AUTO_W);
	ui_btn_set_height(&btn, UI_RENDER_AUTO_H);
	ui_btn_set_style(&btn, UI_BTN_RENDER_AUTO, UI_TXT_AUTO, color_bg);
	ui_btn_set_txt_color(&btn, color_txt);
	ui_push_button(ui, &btn);
}
