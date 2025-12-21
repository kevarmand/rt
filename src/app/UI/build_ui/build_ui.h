/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ui.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 09:00:17 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 09:06:17 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILD_UI_H
# define BUILD_UI_H

# include "../ui_int.h"
# include "../ui_settings.h"
#include "display.h"

void	ui_btn_set_rect(t_button *btn, int x, int y, int width);
void	ui_btn_set_height(t_button *btn, int height);
void	ui_btn_set_style(t_button *btn, int label,
			char *txt, int color_bg);
void	ui_btn_set_txt_color(t_button *btn, int color_txt);
void	ui_colors_from_selected(int selected, int *color_bg, int *color_txt);
void	ui_add_close(t_ui *ui);
void	ui_add_ssaa(t_ui *ui);
void	ui_add_tonemap_label(t_ui *ui);
void	ui_add_tonemap_toggle(t_ui *ui);
void	ui_add_apply(t_ui *ui);
void	ui_add_mode_lock(t_ui *ui);
void	ui_add_mode_roll(t_ui *ui);
void	ui_add_mode_free(t_ui *ui);
void	ui_add_render_auto(t_ui *ui);
void	ui_add_render_fast(t_ui *ui);
void	ui_add_render_normal(t_ui *ui);
void	ui_add_render_super(t_ui *ui);
void	ui_build_buttons(t_ui *ui);
char	*ui_ssaa_txt(int idx);
char	*ui_onoff_txt(int enabled);
int		ui_is_mode_selected(t_ui *ui, int mode_idx);
int		ui_is_render_selected(t_ui *ui, int render_mode);
void	ui_push_button(t_ui *ui, t_button *btn);
void	ui_colors_from_enabled(int enabled, int *color_bg, int *color_txt);

#endif