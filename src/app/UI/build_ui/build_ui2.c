/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_u2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 09:01:39 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 09:01:47 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "build_ui.h"

void	ui_btn_set_rect(t_button *btn, int x, int y, int width)
{
	btn->x = x;
	btn->y = y;
	btn->width = width;
}

void	ui_btn_set_height(t_button *btn, int height)
{
	btn->height = height;
}

void	ui_btn_set_style(t_button *btn, int label, char *txt, int color_bg)
{
	btn->label = label;
	btn->txt = txt;
	btn->color_bg = color_bg;
}

void	ui_btn_set_txt_color(t_button *btn, int color_txt)
{
	btn->color_txt = color_txt;
}

void	ui_colors_from_selected(int selected, int *color_bg, int *color_txt)
{
	*color_bg = UI_COLOR_INACTIVE;
	*color_txt = UI_COLOR_TEXT;
	if (selected)
	{
		*color_bg = UI_COLOR_ACTIVE;
		*color_txt = UI_COLOR_TEXT_SELECTED;
	}
}
