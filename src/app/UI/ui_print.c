/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_print.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 01:53:49 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/20 13:03:43 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ui_int.h"
#include "ui_settings.h"
#include "mlx.h"
#include "new_rt.h"

void	print_txt_ui(t_data *data)
{
	t_display	*display;
	t_ui		*ui;
	int			i;

	display = &data->display;
	ui = &display->ui;
	if (!ui->visible)
		return ;
	mlx_string_put(display->mlx, display->win,
		ui->x + 10, ui->y + 18, UI_COLOR_TEXT, "UI");
	i = 0;
	while (i < ui->button_count)
	{
		if (ui->buttons[i].txt)
		{
			mlx_string_put(display->mlx, display->win,
				ui->x + ui->buttons[i].x + 4,
				ui->y + ui->buttons[i].y + (ui->buttons[i].height / 2) + 4,
				ui->buttons[i].color_txt, ui->buttons[i].txt);
		}
		i++;
	}
	ui_print_cam_status(data);
}
