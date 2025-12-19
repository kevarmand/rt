/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_int.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 01:50:29 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/19 16:37:18 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UI_INT_H
# define UI_INT_H

typedef struct s_display	t_display;
typedef struct s_data		t_data;

# include "image.h"

# define MAX_UI_BUTTONS 16

typedef enum e_ui_btn
{
	UI_BTN_NONE = -1,
	UI_BTN_CLOSE = 1,
	UI_BTN_TONEMAP,
	UI_BTN_SSAA_MINUS,
	UI_BTN_SSAA_VALUE,
	UI_BTN_SSAA_PLUS,
	UI_BTN_MODE_LOCK,
	UI_BTN_MODE_ROLL,
	UI_BTN_MODE_FREE,
	UI_BTN_APPLY

}	t_ui_btn;

typedef struct s_button
{
	int			x;
	int			y;
	int			width;
	int			height;
	int			label;
	const char	*txt;
	int			color_bg;
	int			color_txt;
}	t_button;

typedef struct s_ui
{
	int			visible;
	int			dirty;
	t_image		img;
	int			event;
	int			x;
	int			y;
	int			dragging;
	t_button	buttons[MAX_UI_BUTTONS];
	int			button_count;
	int			mouse_was_down;
	int			ssaa_idx;
	int			tonemap_enabled;
	int			mode_selected;
}	t_ui;

void	init_ui(t_display *display);
void	display_update_ui(t_display *display);
void	print_txt_ui(t_data *data);
void	draw_ui(t_ui *ui);
void	ui_build_buttons(t_ui *ui);

#endif
