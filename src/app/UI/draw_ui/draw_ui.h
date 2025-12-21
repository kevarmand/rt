/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_ui.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 09:09:27 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 09:38:25 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DRAW_UI_H
# define DRAW_UI_H

# include "../ui_int.h"
# include "../ui_settings.h"

typedef struct s_rect
{
	int	x;
	int	y;
	int	w;
	int	h;
	int	color;
}	t_rect;

void	ui_fill_rect(t_image *img, t_rect *rect);
void	ui_draw_base_fill(t_ui *ui);
void	ui_draw_base_border_h(t_ui *ui, int thickness);
void	ui_draw_icon_plus(t_image *img, t_button *btn);
void	ui_draw_icon_minus(t_image *img, t_button *btn);
void	ui_draw_icon_cross(t_image *img, t_button *btn);
void	ui_build_buttons(t_ui *ui);
void	ui_put_pixel(t_image *img, int x, int y, int color);
void	ui_draw_base(t_ui *ui);
void	ui_draw_one_button(t_ui *ui, t_button *btn);

#endif