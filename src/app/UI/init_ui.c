/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_ui.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 01:04:11 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 07:39:39 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ui_int.h"
#include "ui_settings.h"
#include "mlx.h"
#include "errors.h"
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
	ui->render_mode_selected = USER_RENDER_AUTO;
	ui->dirty = 0;
	ui->was_visible = 0;
}

static int	ui_init_image(t_display *display)
{
	t_image	*img;

	img = &display->ui.img;
	img->width = UI_W;
	img->height = UI_H;
	img->img_ptr = mlx_new_image(display->mlx, img->width, img->height);
	if (img->img_ptr == 0)
		return (perr(ERR_MLX, PERR_MLX_NEW_IMG));
	img->data = mlx_get_data_addr(img->img_ptr,
			&img->bpp, &img->size_l, &img->endian);
	return (SUCCESS);
}

int	init_ui(t_display *display)
{
	ui_init_state(&display->ui);
	if(ui_init_image(display))
		return (ERR_MLX);
	ui_build_buttons(&display->ui);
	display->flag_ui = 1;
	display->ui.dirty = 1;
	return (SUCCESS);
}
