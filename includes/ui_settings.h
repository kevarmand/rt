/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_settings.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 22:01:39 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/18 22:02:42 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UI_SETTINGS_H
# define UI_SETTINGS_H

/***
 * Diferent modes for the UI.
 */
enum e_ui_mode {
	UI_MODE_NONE = 0,
	UI_MODE_OBJECT = 1,
	UI_MODE_EDITOR = 2
};

/**
 * Color session for different UI elements.
 */
typedef enum e_ui_color {
	UI_COLOR_FOND = 0x0A64AD,
	UI_COLOR_EXIT = 0xFF2020,
	UI_COLOR_BORDER = 0xF9E7D0,
	UI_COLOR_FIELD_BG = 0xDDDDDD,
	UI_COLOR_FIELD_ACTIVE =0xB0C4DE,
	UI_COLOR_INACTIVE = 0xA9A9A9,
	UI_COLOR_PANEL = 0xD3D3D3,
	UI_COLOR_ACCENT = 0x00FF00,
	UI_COLOR_TEXT = 0xFFFFFF,
	UI_COLOR_TEST_MUTED = 555555
}	t_ui_color;

/*
 * Dimensions and positions for different UI elements.
 */
//Interface
# define WIDTH_UI 200
# define HEIGHT_UI 450

//Standard button size
# define UI_BUTTON_W 50
# define UI_BUTTON_H 16

// Exit button
# define UI_EXIT_W 20
# define UI_EXIT_H 20
# define UI_EXIT_X 175
# define UI_EXIT_Y 5

// TAB buttons
# define UI_TAB_BUTTON_W 40
# define UI_TAB_BUTTON_H 16
# define UI_TAB_BUTTON_X 15
# define UI_TAB_BUTTON_Y 35

// pannel general section
# define UI_PANNEL_X 10
# define UI_PANNEL_Y 51
# define UI_PANNEL_W 180
# define UI_PANNEL_H 350

//pannel description section
# define UI_DESCRIPTION_X 15
# define UI_DESCRIPTION_Y 55
# define UI_DESCRIPTION_W 170
# define UI_DESCRIPTION_H 100

//button Name
# define UI_BUTTON_NAME_X 0
# define UI_BUTTON_NAME_Y 0
# define UI_BUTTON_NAME_W 50
# define UI_BUTTON_NAME_H 16
# define UI_LABEL_NAME        "Name"

//button Next name
# define UI_BUTTON_NEXT_X 60
# define UI_BUTTON_NEXT_Y 0
# define UI_BUTTON_NEXT_W 50
# define UI_BUTTON_NEXT_H 16
# define UI_LABEL_NEXT       "  Next"

//buton Color

# define UI_BUTTON_COLOR_X 0
# define UI_BUTTON_COLOR_Y 20
# define UI_BUTTON_COLOR_W 50
# define UI_BUTTON_COLOR_H 16

# define UI_HEADER_HEIGHT 30
# define UI_CONTENT_X 5
/***
 * Labels for UI elements.
 */
# define UI_LABEL_OBJECT      "Object"
# define UI_LABEL_NAME        "Name"
# define UI_LABEL_TRANSLATION "        Translation"
# define UI_LABEL_ROTATION    "          Rotation"

#endif // UI_SETTINGS_H