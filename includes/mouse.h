/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 23:05:12 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/06 15:10:48 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UI2_H
# define UI2_H

typedef enum e_mouse_mode
{
	MOUSE_MODE_NONE = 0,
	MOUSE_MODE_CAM_FREE = 1,
	MOUSE_MODE_CAM_ROLL = 2,
	MOUSE_MODE_UI = 3
}	t_mouse_mode;

typedef struct s_mouse_state
{
	int		mode;
	int		is_down;
	int		anchor_x;
	int		anchor_y;
	int		current_x;
	int		current_y;
	int		scroll_delta;
	float	roll_prev_angle;
}	t_mouse_state;



#endif