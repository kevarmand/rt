/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_ui.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 11:20:00 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/19 15:28:54 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ui_int.h"
#include "ui_settings.h"

typedef struct s_rect
{
	int	x;
	int	y;
	int	w;
	int	h;
	int	color;
}	t_rect;

static void	ui_put_pixel(t_image *img, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || y < 0 || x >= img->width || y >= img->height)
		return ;
	dst = img->data + (y * img->size_l) + (x * (img->bpp / 8));
	*(unsigned int *)dst = (unsigned int)color;
}

static void	ui_fill_rect(t_image *img, t_rect *rect)
{
	int	xx;
	int	yy;

	yy = 0;
	while (yy < rect->h)
	{
		xx = 0;
		while (xx < rect->w)
		{
			ui_put_pixel(img, rect->x + xx, rect->y + yy, rect->color);
			xx++;
		}
		yy++;
	}
}

static void	ui_draw_base_fill(t_ui *ui)
{
	t_rect	rect;

	rect.x = 0;
	rect.y = 0;
	rect.w = ui->img.width;
	rect.h = ui->img.height;
	rect.color = UI_COLOR_BG;
	ui_fill_rect(&ui->img, &rect);
	rect.x = 0;
	rect.y = 0;
	rect.w = ui->img.width;
	rect.h = UI_HEADER_H;
	rect.color = UI_COLOR_INACTIVE;
	ui_fill_rect(&ui->img, &rect);
}

static void	ui_draw_base_border_h(t_ui *ui, int thickness)
{
	t_rect	rect;

	rect.x = 0;
	rect.y = 0;
	rect.w = ui->img.width;
	rect.h = thickness;
	rect.color = UI_COLOR_INACTIVE;
	ui_fill_rect(&ui->img, &rect);
	rect.x = 0;
	rect.y = ui->img.height - thickness;
	rect.w = ui->img.width;
	rect.h = thickness;
	ui_fill_rect(&ui->img, &rect);
}

static void	ui_draw_base_border_v(t_ui *ui, int thickness)
{
	t_rect	rect;

	rect.x = 0;
	rect.y = 0;
	rect.w = thickness;
	rect.h = ui->img.height;
	rect.color = UI_COLOR_INACTIVE;
	ui_fill_rect(&ui->img, &rect);
	rect.x = ui->img.width - thickness;
	rect.y = 0;
	rect.w = thickness;
	rect.h = ui->img.height;
	ui_fill_rect(&ui->img, &rect);
}

static void	ui_draw_base(t_ui *ui)
{
	int	thickness;

	thickness = UI_BORDER_THICKNESS;
	ui_draw_base_fill(ui);
	ui_draw_base_border_h(ui, thickness);
	ui_draw_base_border_v(ui, thickness);
}

static void	ui_draw_icon_plus(t_image *img, t_button *btn)
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

static void	ui_draw_icon_minus(t_image *img, t_button *btn)
{
	t_rect	rect;

	rect.color = btn->color_txt;
	rect.x = btn->x + UI_ICON_MINUS_X;
	rect.y = btn->y + UI_ICON_MINUS_Y;
	rect.w = UI_ICON_MINUS_W;
	rect.h = UI_ICON_MINUS_H;
	ui_fill_rect(img, &rect);
}

static void	ui_draw_icon_cross(t_image *img, t_button *btn)
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

static void	ui_draw_one_button(t_ui *ui, t_button *btn)
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

void	draw_ui(t_ui *ui)
{
	int	index;

	ui_draw_base(ui);
	index = 0;
	while (index < ui->button_count)
	{
		ui_draw_one_button(ui, &ui->buttons[index]);
		index++;
	}
}
