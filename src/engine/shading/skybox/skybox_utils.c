/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skybox_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 18:40:14 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/18 18:41:48 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"
#include <math.h>

float	clampf(float value, float min_value, float max_value)
{
	if (value < min_value)
		return (min_value);
	if (value > max_value)
		return (max_value);
	return (value);
}

float	smoothstep(float edge0, float edge1, float value)
{
	float	t;

	t = (value - edge0) / (edge1 - edge0);
	t = clampf(t, 0.0f, 1.0f);
	return (t * t * (3.0f - 2.0f * t));
}

void	sky_gradient_eval(t_vec3f dir, t_vec3f *out)
{
	t_vec3f	color_top;
	t_vec3f	color_mid;
	t_vec3f	color_bot;
	float	height;
	float	k;

	color_top = (t_vec3f){0.01f, 0.02f, 0.05f};
	color_mid = (t_vec3f){0.08f, 0.12f, 0.20f};
	color_bot = (t_vec3f){0.55f, 0.35f, 0.25f};
	height = clampf(dir[1] * 0.5f + 0.5f, 0.0f, 1.0f);
	if (height < 0.55f)
	{
		k = height / 0.55f;
		*out = vec3f_lerp(color_bot, color_mid, k);
		return ;
	}
	k = (height - 0.55f) / 0.45f;
	*out = vec3f_lerp(color_mid, color_top, k);
}
