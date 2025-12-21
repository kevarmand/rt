/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 09:12:50 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 09:25:31 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CORE_H
# define CORE_H

#include "../ui_int.h"
#include "../ui_settings.h"
#include "display.h"
#include <stdio.h>

int		ui_mouse_just_pressed(t_ui *ui, t_mouse_state *mouse);
int		ui_mouse_in_header(t_ui *ui, t_mouse_state *mouse);
void	ui_try_start_drag(t_ui *ui, t_mouse_state *mouse);
int		ui_clamp_int(int value, int min_v, int max_v);
int		ui_drag_step(t_display *display, t_mouse_state *mouse);
int		ui_mouse_in_button(t_ui *ui, t_mouse_state *mouse, t_button *btn);
int		ui_is_clickable(t_button *btn);
int		ui_try_click_button(t_ui *ui, t_mouse_state *mouse);
void	ui_apply_event(t_ui *ui, int event_id);
void	apply_event(t_display *display, t_ui *ui);

#endif