/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_ui2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 09:10:49 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 09:27:00 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "draw_ui.h"

void	ui_put_pixel(t_image *img, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || y < 0 || x >= img->width || y >= img->height)
		return ;
	dst = img->data + (y * img->size_l) + (x * (img->bpp / 8));
	*(unsigned int *)dst = (unsigned int)color;
}

void	ui_draw_icon_plus(t_image *img, t_button *btn)
{
	t_rect	rect;

	rect.color = btn->color_txt;
	rect.x = btn->x + UI_ICON_PLUS_VX;
	rect.y = btn->y + UI_ICON_PLUS_VY;
	rect.w = UI_ICON_PLUS_VW;
	rect.h = UI_ICON_PLUS_VH;
	ui_fill_rect(img, &rect);
	rect.x = btn->x + UI_ICON_PLUS_HX;
	rect.y = btn->y + UI_ICON_PLUS_HY;
	rect.w = UI_ICON_PLUS_HW;
	rect.h = UI_ICON_PLUS_HH;
	ui_fill_rect(img, &rect);
}

void	ui_draw_icon_minus(t_image *img, t_button *btn)
{
	t_rect	rect;

	rect.color = btn->color_txt;
	rect.x = btn->x + UI_ICON_MINUS_X;
	rect.y = btn->y + UI_ICON_MINUS_Y;
	rect.w = UI_ICON_MINUS_W;
	rect.h = UI_ICON_MINUS_H;
	ui_fill_rect(img, &rect);
}

void	ui_draw_icon_cross(t_image *img, t_button *btn)
{
	int	i;

	i = UI_ICON_X_MARGIN;
	while (i < btn->width - UI_ICON_X_MARGIN)
	{
		ui_put_pixel(img, btn->x + i, btn->y + i, btn->color_txt);
		ui_put_pixel(img, btn->x + i, btn->y + i + 1, btn->color_txt);
		ui_put_pixel(img, btn->x + (btn->width - 1 - i),
			btn->y + i, btn->color_txt);
		ui_put_pixel(img, btn->x + (btn->width - 1 - i),
			btn->y + i + 1, btn->color_txt);
		i++;
	}
}

void	ui_draw_one_button(t_ui *ui, t_button *btn)
{
	t_rect	rect;

	rect.x = btn->x;
	rect.y = btn->y;
	rect.w = btn->width;
	rect.h = btn->height;
	rect.color = btn->color_bg;
	ui_fill_rect(&ui->img, &rect);
	if (btn->label == UI_BTN_SSAA_PLUS)
		ui_draw_icon_plus(&ui->img, btn);
	else if (btn->label == UI_BTN_SSAA_MINUS)
		ui_draw_icon_minus(&ui->img, btn);
	else if (btn->label == UI_BTN_CLOSE)
		ui_draw_icon_cross(&ui->img, btn);
}
