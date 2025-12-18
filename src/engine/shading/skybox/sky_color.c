/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sky_color.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 17:09:21 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/18 17:13:13 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"

static float	sky_height(const t_vec3f direction)
{
	if (direction[1] < 0.0f)
		return (-direction[1]);
	return (direction[1]);
}

static void	sky_mix(const t_vec3f color_from, const t_vec3f color_to,
				float factor, t_vec3f *color_out)
{
	float	inverse;

	inverse = 1.0f - factor;
	(*color_out)[0] = color_from[0] * inverse + color_to[0] * factor;
	(*color_out)[1] = color_from[1] * inverse + color_to[1] * factor;
	(*color_out)[2] = color_from[2] * inverse + color_to[2] * factor;
}

void	sky_color(const t_vec3f direction, t_vec3f *color_out)
{
	const t_vec3f	color_night = {0.01f, 0.02f, 0.06f};
	const t_vec3f	color_blue = {0.40f, 0.70f, 1.00f};
	const t_vec3f	color_sunset = {1.00f, 0.55f, 0.40f};
	float			height;
	float			factor;

	height = sky_height(direction);
	if (height < 0.6f)
	{
		factor = height / 0.6f;
		sky_mix(color_sunset, color_blue, factor, color_out);
	}
	else
	{
		factor = (height - 0.6f) / 0.4f;
		sky_mix(color_blue, color_night, factor, color_out);
	}
}
