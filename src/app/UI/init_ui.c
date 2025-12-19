/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_ui.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 01:04:11 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/19 15:38:56 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ui_int.h"
#include "ui_settings.h"
#include "mlx.h"
#include "display.h"

static void	ui_init_state(t_ui *ui)
{
	ui->visible = 0;
	ui->event = -1;
	ui->x = UI_POS_X_DEFAULT;
	ui->y = UI_POS_Y_DEFAULT;
	ui->dragging = 0;
	ui->button_count = 0;
	ui->mouse_was_down = 0;
	ui->ssaa_idx = 0;
	ui->tonemap_enabled = 0;
	ui->mode_selected = 0;
}

static void	ui_init_image(t_display *display)
{
	t_image	*img;

	img = &display->ui.img;
	img->width = UI_W;
	img->height = UI_H;
	img->img_ptr = mlx_new_image(display->mlx, img->width, img->height);
	img->data = mlx_get_data_addr(img->img_ptr,
			&img->bpp, &img->size_l, &img->endian);
}

void	init_ui(t_display *display)
{
	ui_init_state(&display->ui);
	ui_init_image(display);
	ui_build_buttons(&display->ui);
	display->flag_ui = 1;
	display->ui.dirty = 1;
}
