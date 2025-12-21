/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_ui1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 09:08:05 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 09:38:36 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "draw_ui.h"

void	ui_fill_rect(t_image *img, t_rect *rect)
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

void	ui_draw_base_fill(t_ui *ui)
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

void	ui_draw_base_border_h(t_ui *ui, int thickness)
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

void	ui_draw_base_border_v(t_ui *ui, int thickness)
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

void	ui_draw_base(t_ui *ui)
{
	int	thickness;

	thickness = UI_BORDER_THICKNESS;
	ui_draw_base_fill(ui);
	ui_draw_base_border_h(ui, thickness);
	ui_draw_base_border_v(ui, thickness);
}
