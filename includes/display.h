/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 12:27:50 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/16 17:21:56 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DISPLAY_H
# define DISPLAY_H

	// # include "ui.h"
# include "mouse.h"
# include "cam_ctrl.h"

typedef struct s_data t_data;

typedef struct s_image
{
	void	*img_ptr;
	char	*data;
	int		bpp;
	int		size_l;
	int		endian;
	int		width;
	int		height;
}	t_image;

typedef enum e_quality
{
	QUALITY_NONE = 0,   /* pas d'image ou obsolète */
	QUALITY_FAST = 1,
	QUALITY_NORMAL = 2,
	QUALITY_SUPER = 3,
}	t_quality;

typedef enum e_user_render_mode
{
	USER_RENDER_AUTO = 0,
	USER_RENDER_LOCK_FAST = 1,
	USER_RENDER_LOCK_NORMAL = 2,
	USER_RENDER_LOCK_SUPER = 3
}	t_user_render_mode;


typedef struct s_frame
{
	int			*rgb_pixels;
	t_quality	quality;
}	t_frame;

typedef struct s_ui
{
	int 	visible;
}	t_ui;


typedef struct s_ds_sync
{
	int	in_flight;
	int	pending;
	int	req_mode;
	int	req_cam;
}	t_ds_sync;


/**
 * @brief Display system state and communication flags.
 *
 * This structure contains all the state required by the MLX thread for:
 *  - camera handling,
 *  - progressive rendering,
 *  - UI refresh management,
 *  - communication with the render engine.
 *
 * The system follows a strict pipeline, executed once per frame:
 *    1. display_update_camera()
 *    2. display_prepare_render()     (optional module)
 *    3. engine_sync_display()
 *    4. display_update_main_image()
 *    5. display_update_ui()
 *    6. display_draw_base()
 *    7. display_draw_ui()
 *
 * Each flag has a well-defined ownership. Only the designated module is
 * allowed to modify it. This keeps the system predictable and prevents
 * invalid states.
 *
 * --------------------------------------------------------------------------
 * Flag ownership and responsibilities
 * --------------------------------------------------------------------------
 *
 * flag_camera_changed
 * -------------------
 *  - Set to 1 by input hooks (mouse, keyboard) whenever the user changes the
 *    current camera's parameters (position, direction, FOV, index switch).
 *  - Consumed and reset to 0 by display_update_camera().
 *  - This flag never directly triggers rendering; it only signals that the
 *    camera parameters have changed.
 *
 *
 * flag_camera_level
 * -----------------
 *  - Set to 1 by input/UI when the rendering quality level is modified
 *    (FAST → NORMAL → SUPER).
 *  - Consumed and reset to 0 by display_prepare_render().
 *  - Used to determine whether an existing cached image is sufficient or
 *    whether a new render must be launched.
 *
 *
 * flag_img
 * --------
 *  - Set to 1 exclusively by engine_sync_display() when a new RGB buffer has
 *    been copied into display_pixels and must be presented on screen.
 *  - Consumed and reset to 0 by display_update_main_image() once the image
 *    has been uploaded to the MLX texture.
 *  - No other module is allowed to modify this flag.
 *
 *
 * flag_ui
 * -------
 *  - Set by engine_sync_display() or UI modules when the user interface
 *    requires a redraw (progress bar update, camera info, etc.).
 *  - Consumed by display_update_ui().
 *  - Behavior similar to flag_img but reserved for UI widgets.
 *
 *
 * cam_to_render
 * -------------
 *  - Owned exclusively by engine_sync_display().
 *  - Holds the index of the camera currently being rendered by the engine.
 *  - Set when a render request is sent; reset to -1 when a full frame is
 *    received and stored.
 *  - No other module must modify this value.
 *
 *
 * frame[i].is_dirty
 * -----------------
 *  - Indicates whether camera i requires a full rerender.
 *  - Set only by engine_sync_display(), based on high-level flags coming
 *    from display_prepare_render().
 *  - Reset only by engine_sync_display() when the engine delivers a complete
 *    image for that camera.
 *  - This field is never written directly by UI or input hooks.
 *
 *
 * display_pixels
 * --------------
 *  - Temporary buffer used for progressive / partial updates.
 *  - Written by engine_sync_display() whenever new tiles arrive.
 *  - Read by display_update_main_image().
 *
 *
 * frame[i].rgb_pixels
 * -------------------
 *  - Persistent “clean” image for camera i.
 *  - Written only when a full frame is completed.
 *  - Used for instant camera switching without rerendering.
 *
 */
typedef struct s_display
{
	void	*mlx;
	void	*win;
	t_image	main_img;
	 t_ui	ui;

	t_frame	*frame;
	int		total_cams;
	int		pixel_count;

	int		current_cam;
	int		cam_to_render;

	int		flag_ui;
	int		flag_camera_changed;
	int		flag_request_render;
	int		flag_img_buffer;
	int		flag_img_window;
	int		user_render_mode;
	t_ds_sync	ds_sync;
	t_mouse_state	mouse;
	t_cam_ctrl		cam_ctrl;
}	t_display;


/* ************************************************************************** */
/*         					     DISPLAY FUNCTIONS                            */
/* ************************************************************************** */

/***
 * Initializes the display system.
 * Allocates necessary resources and sets up the main window and image.
 * @param display Pointer to the display structure to initialize.
 * @param data Pointer to the main data structure containing scene and engine info.
 * @return SUCCESS on success, error code otherwise.
 */
int		display_init(t_display *display, t_data *data);


void	display_update_camera(t_data *data);

#endif