/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 17:00:30 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/16 17:01:07 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "color.h"

float	srgb8_to_linear(int c)
{
	float	x;

	x = (float)c / 255.0f;
	if (x <= 0.04045f)
		return (x / 12.92f);
	return ((float)powf((x + 0.055f) / 1.055f, 2.4f));
}

int	linear_to_srgb8(float x)
{
	float	y;
	int		u;

	if (x <= 0.0f)
		return (0);
	if (x >= 1.0f)
		return (255);
	if (x <= 0.0031308f)
		y = 12.92f * x;
	else
		y = 1.055f * (float)powf(x, 1.0f / 2.4f) - 0.055f;
	u = (int)(y * 255.0f + 0.5f);
	if (u < 0)
		u = 0;
	if (u > 255)
		u = 255;
	return (u);
}

void	rgb8_to_linear_vec(const int rgb[3], t_vec3f *out)
{
	(*out)[0] = srgb8_to_linear(rgb[0]);
	(*out)[1] = srgb8_to_linear(rgb[1]);
	(*out)[2] = srgb8_to_linear(rgb[2]);
}

void	linear_to_rgb8_vec(const t_vec3f *lin, int rgb_out[3])
{
	rgb_out[0] = linear_to_srgb8((*lin)[0]);
	rgb_out[1] = linear_to_srgb8((*lin)[1]);
	rgb_out[2] = linear_to_srgb8((*lin)[2]);
}
