/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_cam_status.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 14:30:00 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/20 14:30:00 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ui_int.h"
#include "display.h"
#include "mlx.h"
#include "new_rt.h"

static int	ui_quality_color(t_quality quality)
{
	if (quality == QUALITY_FAST)
		return (0x00FF00);
	if (quality == QUALITY_NORMAL)
		return (0xFFFFFF);
	if (quality == QUALITY_SUPER)
		return (0xFFA500);
	return (0xFF0000);
}

static int	ui_itoa(int value, char *buf)
{
	int	len;
	int tmp;
    int out_len;

	if (value == 0)
	{
		buf[0] = '0';
		buf[1] = '\0';
		return (1);
	}
	len = 0;
	tmp = value;
	while (tmp > 0)
	{
		tmp /= 10;
		len++;
	}
    out_len = len;
	buf[len] = '\0';
	while (value > 0)
	{
		buf[--len] = '0' + (value % 10);
		value /= 10;
	}
	return (out_len);
}


#include <stdio.h>
static void	ui_print_one_cam(t_display *display, int index, int x, int y)
{
	t_frame	*frame;
	char	buf[32];
	int		color;
	int		len;

	frame = &display->frame[index];
	color = ui_quality_color(frame->quality);
	buf[0] = (index == display->current_cam) ? '*' : ' ';
	buf[1] = ' ';
	buf[2] = 'c';
	buf[3] = 'a';
	buf[4] = 'm';
	buf[5] = ' ';
	buf[6] = '0' + (index + 1);
	buf[7] = ' ';
	buf[8] = ':';
	buf[9] = ' ';
	len = 10;
	len += ui_itoa(frame->tiles_received, buf + len);
	buf[len++] = ' ';
	buf[len++] = '/';
	buf[len++] = ' ';
	len += ui_itoa(frame->tiles_total, buf + len);
	buf[len] = '\0';
	mlx_string_put(display->mlx, display->win, x, y, color, buf);
}


static void	ui_cam_window_range(t_display *display, int *start, int *end)
{
	int	total;
	int	left;

	total = display->total_cams;
	*start = 0;
	*end = total - 1;
	if (total <= 5)
		return ;
	left = display->current_cam - 2;
	if (left < 0)
		left = 0;
	*start = left;
	*end = *start + 4;
	if (*end > total - 1)
	{
		*end = total - 1;
		*start = *end - 4;
	}
}

void	ui_print_cam_status(t_data *data)
{
	t_display	*display;
	int			start;
	int			end;
	int			index;
	int			x;
	int			y;
	int			lines;

	display = &data->display;
	ui_cam_window_range(display, &start, &end);
	lines = end - start + 1;
	if (lines <= 0)
		return ;
	if (display->main_img.height < lines * 12 + 8)
		return ;
	x = 10;
	y = display->main_img.height - 12 - (lines - 1) * 12;
	index = start;
	while (index <= end)
	{
		ui_print_one_cam(display, index, x, y);
		y += 12;
		index++;
	}
}
