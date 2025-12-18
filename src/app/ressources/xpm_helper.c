/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xpm_helper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 17:59:50 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/17 18:08:55 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "xpm.h"

void	extract_rgb(int pixel_value, int rgb[3])
{
	rgb[0] = (pixel_value >> 16) & 0xFF;
	rgb[1] = (pixel_value >> 8) & 0xFF;
	rgb[2] = pixel_value & 0xFF;
}

float	rgb8_to_height(int rgb[3])
{
	return ((float)(rgb[0] + rgb[1] + rgb[2]) / 765.0f);
}

int	wrap_index(int value, int size)
{
	value = value % size;
	if (value < 0)
		value += size;
	return (value);
}

float	height_at_z(const t_vec3f *pixels, int width, int x, int y)
{
	return (pixels[y * width + x][2]);
}

void	bump_clear_z(t_texture *tex)
{
	int		texel_index;
	int		count;
	t_vec3f	*pixels;

	pixels = (t_vec3f *)tex->pixels;
	count = tex->width * tex->height;
	texel_index = 0;
	while (texel_index < count)
	{
		pixels[texel_index][2] = 0.0f;
		texel_index++;
	}
}
