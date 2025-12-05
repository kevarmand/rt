/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 23:05:12 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/05 23:11:19 by kearmand         ###   ########.fr       */
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
	int	mode;      /* t_mouse_mode */
	int	is_down;
	int	last_x;
	int	last_y;
	int	accum_dx;
	int	accum_dy;
	int	scroll_delta;
}	t_mouse_state;


#endif