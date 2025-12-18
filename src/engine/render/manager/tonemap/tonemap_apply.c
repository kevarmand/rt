/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tonemap_apply.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 17:15:24 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/17 17:15:25 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tonemap_apply.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 15:41:36 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/17 17:05:34 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tonemap_internal.h"
#include "color.h"
#include <math.h>

float	tm_exposure_from_lum(float lum_ref, float key)
{
	if (lum_ref <= TM_EPS)
		return (1.0f);
	return (key / lum_ref);
}

float	tm_filmic_exp(float lum, float exposure)
{
	float	x;

	x = lum * exposure;
	if (x <= 0.0f)
		return (0.0f);
	return (1.0f - expf(-x));
}

void	tm_apply_pixel(t_mgr *mgr, int index, float exposure)
{
	t_vec3f	color;
	float	lum;
	float	mapped;
	float	scale;
	int		rgb[3];

	color = mgr->hdr_buffer[index];
	lum = tm_luminance(&color);
	mapped = tm_filmic_exp(lum, exposure);
	if (lum > TM_EPS)
		scale = mapped / lum;
	else
		scale = 0.0f;
	color[0] *= scale;
	color[1] *= scale;
	color[2] *= scale;
	linear_to_rgb8_vec(&color, rgb);
	mgr->rgb_buffer[index] = (rgb[0] << 16) | (rgb[1] << 8) | rgb[2];
}

void	tm_apply_full_image(t_mgr *mgr, int width, int height, float exposure)
{
	int	x;
	int	y;
	int	index;

	y = 0;
	while (y < height)
	{
		x = 0;
		while (x < width)
		{
			index = y * width + x;
			tm_apply_pixel(mgr, index, exposure);
			x++;
		}
		y++;
	}
}
