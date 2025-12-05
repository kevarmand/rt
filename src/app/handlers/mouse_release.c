/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_release.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 19:18:17 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/05 16:07:16 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "new_rt.h"
#include "display.h"

// int	mouse_release(int button, int x, int y, t_data *data)
// {
// 	(void)x;
// 	(void)y;
// 	if (button == 1 && data->display.ui.dragging)
// 	{
// 		data->display.ui.dragging = 0;
// 		data->display.flag_ui = 1;
// 	}
// 	return (0);
// }

int	mouse_release(int button, int x, int y, t_data *data)
{
	t_display	*display;

	(void)x;
	(void)y;
	display = &data->display;
	if (button == 1)
	{
		display->mouse.is_down = 0;
		if (display->mouse.mode == 2)
			display->mouse.mode = 0;
	}
	return (0);
}
