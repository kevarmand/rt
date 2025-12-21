/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_move.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 19:18:14 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 08:01:50 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "new_rt.h"
#include "display.h"

int	mouse_move(int x, int y, t_data *data)
{
	t_mouse_state	*mouse;

	mouse = &data->display.mouse;
	mouse->current_x = x;
	mouse->current_y = y;
	return (0);
}
