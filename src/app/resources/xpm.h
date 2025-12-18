/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xpm.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 17:57:47 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/17 18:05:52 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef XPM_H
# define XPM_H

# include "scene.h"
# include "display.h"
# include "mlx.h"
# include <stdlib.h>
# include <math.h>
# include "libft.h"
# include "color.h"
# include <stdio.h>

//Helper functions
void	extract_rgb(int pixel_value, int rgb[3]);
float	rgb8_to_height(int rgb[3]);
int		wrap_index(int value, int size);
float	height_at_z(const t_vec3f *pixels, int width, int x, int y);
void	bump_clear_z(t_texture *tex);	

//Albedo loading
void	copy_albedo_from_mlx(t_texture *tex, char *addr,
			int bpp_line[2]);

//Bump loading
void	bump_store_height_z(t_texture *tex, char *addr,
			int bpp_line[2]);
void	bump_bake_dxdy_xy(t_texture *tex);

#endif