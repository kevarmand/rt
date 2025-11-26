/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 12:27:50 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/26 14:45:46 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DISPLAY_H
# define DISPLAY_H

# include "ui.h"
# include "flag.h"

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
	t_vec3f			*hdr_pixels; // buffer calcul interne
	int				width;
	int				height;
	int				samples;
	int				is_dirty;
}	t_frame;

typedef struct s_display
{
	void	*mlx;
	void	*win;
	t_image	main_img;
	t_ui	ui;
	t_frame	*frame;//tableau de frame une par cam

	int		flag;
}	t_display;

#endif