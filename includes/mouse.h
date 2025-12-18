/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 23:05:12 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/18 21:55:04 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MOUSE_H
# define MOUSE_H

typedef enum e_mouse_button
{
	MOUSE_BUTTON_NONE = 0,
	MOUSE_BUTTON_LEFT,
	MOUSE_BUTTON_RIGHT,
}	t_mouse_button;

typedef struct s_mouse_state
{
	int		button;
	int		is_down;
	int		anchor_x;
	int		anchor_y;
	int		current_x;
	int		current_y;
	int		scroll_delta;
}	t_mouse_state;

#endif