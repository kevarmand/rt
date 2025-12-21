/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_settings.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 01:10:48 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 07:03:39 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UI_SETTINGS_H
# define UI_SETTINGS_H

/* =========================
**  UI PANEL SIZE / POSITION
** ========================= */
# define UI_W 200
# define UI_H 180
# define UI_POS_X_DEFAULT 50
# define UI_POS_Y_DEFAULT 50
# define UI_HEADER_H 30

/* =========================
**  UI STATE / INDICES
** ========================= */
# define UI_MODE_IDX_LOCK 0
# define UI_MODE_IDX_ROLL 1
# define UI_MODE_IDX_FREE 2

# define UI_SSAA_IDX_MIN 0
# define UI_SSAA_IDX_MAX 4

/* =========================
**  COLORS
** ========================= */
typedef enum e_ui_color
{
	UI_COLOR_INACTIVE = 0x333333,
	UI_COLOR_ACTIVE = 0x00AAFF,
	UI_COLOR_TEXT = 0xFFFFFF,
	UI_COLOR_VALUE_BG = 0xDDDDDD,
	UI_COLOR_VALUE_TEXT = 0x000000,
	UI_COLOR_EXIT_BG = 0xFF2020,
	UI_COLOR_BG = 0x222222
}	t_ui_color;

# define UI_COLOR_TEXT_SELECTED UI_COLOR_VALUE_TEXT

/* =========================
**  TEXTS
** ========================= */
# define UI_TXT_CLOSE ""
# define UI_TXT_TONEMAP "Tonemap"
# define UI_TXT_SSAA_MINUS ""
# define UI_TXT_SSAA_PLUS ""
# define UI_TXT_LOCK "LOCK"
# define UI_TXT_ROLL "ROLL"
# define UI_TXT_FREE "FREE"
# define UI_TXT_ON "ON"
# define UI_TXT_OFF "OFF"
# define UI_TXT_AUTO "AUTO"
# define UI_TXT_FAST "FAST"
# define UI_TXT_NORMAL "NORM"
# define UI_TXT_SUPER "SSAA"

/* =========================
**  BUTTON LAYOUT
** ========================= */

/* Close button */
# define UI_EXIT_X 172
# define UI_EXIT_Y 5
# define UI_EXIT_W 20
# define UI_EXIT_H 20

/* Tonemap : label + toggle */
# define UI_TONEMAP_LABEL_X 10
# define UI_TONEMAP_LABEL_Y 40
# define UI_TONEMAP_LABEL_W 80
# define UI_TONEMAP_LABEL_H 16

# define UI_TONEMAP_GAP 6
# define UI_TONEMAP_TOGGLE_W 40
# define UI_TONEMAP_TOGGLE_H UI_TONEMAP_LABEL_H
# define UI_TONEMAP_TOGGLE_X 96
# define UI_TONEMAP_TOGGLE_Y UI_TONEMAP_LABEL_Y

/* Render mode */
# define UI_RENDER_AUTO_X 10
# define UI_RENDER_AUTO_Y 70
# define UI_RENDER_AUTO_W 40
# define UI_RENDER_AUTO_H 16

# define UI_RENDER_FAST_X 55
# define UI_RENDER_FAST_Y 70
# define UI_RENDER_FAST_W 40
# define UI_RENDER_FAST_H 16

# define UI_RENDER_NORMAL_X 100
# define UI_RENDER_NORMAL_Y 70
# define UI_RENDER_NORMAL_W 50
# define UI_RENDER_NORMAL_H 16

# define UI_RENDER_SUPER_X 155
# define UI_RENDER_SUPER_Y 70
# define UI_RENDER_SUPER_W 40
# define UI_RENDER_SUPER_H 16

/* SSAA */
# define UI_SSAA_MINUS_X 10
# define UI_SSAA_MINUS_Y 90
# define UI_SSAA_MINUS_W 16
# define UI_SSAA_MINUS_H 16

# define UI_SSAA_VALUE_X 30
# define UI_SSAA_VALUE_Y 90
# define UI_SSAA_VALUE_W 40
# define UI_SSAA_VALUE_H 16

# define UI_SSAA_PLUS_X 75
# define UI_SSAA_PLUS_Y 90
# define UI_SSAA_PLUS_W 16
# define UI_SSAA_PLUS_H 16

/* Camera mode */
# define UI_MODE_LOCK_X 10
# define UI_MODE_LOCK_Y 120
# define UI_MODE_LOCK_W 50
# define UI_MODE_LOCK_H 16

# define UI_MODE_ROLL_X 65
# define UI_MODE_ROLL_Y 120
# define UI_MODE_ROLL_W 50
# define UI_MODE_ROLL_H 16

# define UI_MODE_FREE_X 120
# define UI_MODE_FREE_Y 120
# define UI_MODE_FREE_W 50
# define UI_MODE_FREE_H 16

/* Apply button */
# define UI_APPLY_W 60
# define UI_APPLY_H 18
# define UI_APPLY_X 70
# define UI_APPLY_Y 145
# define UI_TXT_APPLY "APPLY"

/* =========================
**  DRAW SETTINGS (ICONS / FRAME)
** ========================= */
# define UI_BORDER_THICKNESS 3

/* Plus icon (relative to button x/y) */
# define UI_ICON_PLUS_VX 7
# define UI_ICON_PLUS_VY 2
# define UI_ICON_PLUS_VW 2
# define UI_ICON_PLUS_VH 12
# define UI_ICON_PLUS_HX 2
# define UI_ICON_PLUS_HY 7
# define UI_ICON_PLUS_HW 12
# define UI_ICON_PLUS_HH 2

/* Minus icon (relative to button x/y) */
# define UI_ICON_MINUS_X 2
# define UI_ICON_MINUS_Y 7
# define UI_ICON_MINUS_W 12
# define UI_ICON_MINUS_H 2

/* Cross icon (relative to button x/y) */
# define UI_ICON_X_MARGIN 2

#endif
