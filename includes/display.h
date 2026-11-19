/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 12:27:50 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 08:05:55 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DISPLAY_H
# define DISPLAY_H

# include "mouse.h"
# include "cam_ctrl.h"
# include "../src/app/UI/ui_int.h"
# include "image.h"
# include "postprocess.h"

typedef struct s_data	t_data;

typedef enum e_quality
{
	QUALITY_NONE = 0,
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

typedef enum e_frame_state
{
	FRAME_EMPTY = 0,
	FRAME_DIRTY,
	FRAME_RENDERING,
	FRAME_READY
}	t_frame_state;

typedef struct s_frame
{
	int			*rgb_pixels;
	t_quality	quality;
	t_frame_state	state;
	int			tiles_received;
	int			tiles_total;
	int			auto_ev;
}	t_frame;

typedef struct s_ds_sync
{
	int	in_flight;
	int	running_cam;
	int	pending;
	int	req_mode;
	int	req_cam;
}	t_ds_sync;

/*
 * MLX thread owns frame states, quality and RGB caches.
 * sync_engine_tick consumes invalidation flags before accepting snapshots.
 * The atomic mailbox array is the sole authority for expected frame_seq.
 * A dirty frame may retain old pixels as a visual fallback, never as a
 * valid result. READY means a complete matching snapshot was accepted.
 * flag_img_buffer requests an MLX image upload; flag_img_window redraws it.
 */
typedef struct s_display
{
	void			*mlx;
	void			*win;
	t_image			main_img;
	t_ui			ui;

	t_frame			*frame;
	int				total_cams;
	int				pixel_count;

	int				current_cam;

	int				flag_camera_changed;
	int				flag_render_config_changed;
	int				flag_img_buffer;
	int				flag_img_window;
	int				user_render_mode;
	int				render_ssaa;
	int				post_gamma_step;
	int				post_ev_step;
	unsigned int	post_word;
	t_ds_sync		ds_sync;
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
 * @param data Pointer to the main data structure containing scene and 
 * engine info.
 * @return SUCCESS on success, error code otherwise.
 */
int		display_init(t_display *display, t_data *data);

void	display_update_camera(t_data *data);

#endif