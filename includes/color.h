/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 21:39:08 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/18 21:39:22 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLOR_H
# define COLOR_H

# include <stddef.h>
# include "vector.h"

float	srgb8_to_linear(int c);
int		linear_to_srgb8(float x);
void	rgb8_to_linear_vec(const int rgb[3], t_vec3f *out);
void	linear_to_rgb8_vec(const t_vec3f *lin, int rgb_out[3]);

#endif
