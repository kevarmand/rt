/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 12:27:50 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/06 19:19:47 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DISPLAY_H
# define DISPLAY_H

	// # include "ui.h"
# include "flag.h"
# include "mouse.h"

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


typedef struct s_frame
{
	int				*rgb_pixels; // buffer calcul interne
	int				is_dirty;
}	t_frame;

typedef struct s_ui
{
	int 	visible;
}	t_ui;

typedef struct s_display
{
	void	*mlx;
	void	*win;
	t_image	main_img;
	 t_ui	ui;

	t_frame	*frame;         // tableau: 1 frame par caméra
	int		total_cams;
	int		pixel_count;
	int		*display_pixels;

	int		current_cam;    // caméra affichée
	int		cam_to_render;  // caméra actuellement rendue par le manager, ou -1

	int		flag_img;       // 1 = l'image principale doit être redessinée
	int		flag_ui;        // 1 = l'UI doit être redessinée
	int		flag_camera_changed;
	int		flag_camera_level;
	t_mouse_state	mouse;
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