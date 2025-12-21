/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ui.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 15:13:29 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 09:04:05 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "build_ui.h"

void	ui_add_render_fast(t_ui *ui)
{
	t_button	btn;
	int			color_bg;
	int			color_txt;

	ui_colors_from_selected(ui_is_render_selected(ui, USER_RENDER_LOCK_FAST),
		&color_bg, &color_txt);
	ui_btn_set_rect(&btn, UI_RENDER_FAST_X, UI_RENDER_FAST_Y, UI_RENDER_FAST_W);
	ui_btn_set_height(&btn, UI_RENDER_FAST_H);
	ui_btn_set_style(&btn, UI_BTN_RENDER_FAST, UI_TXT_FAST, color_bg);
	ui_btn_set_txt_color(&btn, color_txt);
	ui_push_button(ui, &btn);
}

void	ui_add_render_normal(t_ui *ui)
{
	t_button	btn;
	int			color_bg;
	int			color_txt;

	ui_colors_from_selected(ui_is_render_selected(ui, USER_RENDER_LOCK_NORMAL),
		&color_bg, &color_txt);
	ui_btn_set_rect(&btn, UI_RENDER_NORMAL_X, UI_RENDER_NORMAL_Y,
		UI_RENDER_NORMAL_W);
	ui_btn_set_height(&btn, UI_RENDER_NORMAL_H);
	ui_btn_set_style(&btn, UI_BTN_RENDER_NORMAL, UI_TXT_NORMAL, color_bg);
	ui_btn_set_txt_color(&btn, color_txt);
	ui_push_button(ui, &btn);
}

void	ui_add_render_super(t_ui *ui)
{
	t_button	btn;
	int			color_bg;
	int			color_txt;

	ui_colors_from_selected(ui_is_render_selected(ui, USER_RENDER_LOCK_SUPER),
		&color_bg, &color_txt);
	ui_btn_set_rect(&btn, UI_RENDER_SUPER_X, UI_RENDER_SUPER_Y,
		UI_RENDER_SUPER_W);
	ui_btn_set_height(&btn, UI_RENDER_SUPER_H);
	ui_btn_set_style(&btn, UI_BTN_RENDER_SUPER, UI_TXT_SUPER, color_bg);
	ui_btn_set_txt_color(&btn, color_txt);
	ui_push_button(ui, &btn);
}

void	ui_build_buttons(t_ui *ui)
{
	ui->button_count = 0;
	ui_add_close(ui);
	ui_add_tonemap_label(ui);
	ui_add_tonemap_toggle(ui);
	ui_add_render_auto(ui);
	ui_add_render_fast(ui);
	ui_add_render_normal(ui);
	ui_add_render_super(ui);
	ui_add_ssaa(ui);
	ui_add_mode_lock(ui);
	ui_add_mode_roll(ui);
	ui_add_mode_free(ui);
	ui_add_apply(ui);
}
