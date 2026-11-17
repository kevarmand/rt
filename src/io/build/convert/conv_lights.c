/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conv_lights.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 16:32:30 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 06:58:23 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scene.h"
#include "convert.h"
#include "io.h"
#include "errors.h"
#include <stdlib.h>
#include <string.h>
#include "color.h"
#include "vector.h"

void	light_from_parsed(t_parsed_element *src, t_light *dst)
{
	vec3f_load3(&dst->position, src->u_data.light.position);
	rgb8_to_linear_vec(src->u_data.light.rgb, &dst->color);
	dst->intensity = src->u_data.light.brightness;
}

static void	reset_light(t_light *light)
{
	ft_memset(light, 0, sizeof(t_light));
}

int	conv_lights(t_scene_parsed *parsed, t_conv_ctx *cx)
{
	size_t		i;
	t_light		light_tmp;

	if (vector_reserve(&cx->light_v, cx->light_count) != SUCCESS)
		return (perr(ERR_MALLOC, PERR_M_VEC_LIGHT));
	i = 0;
	while (i < parsed->lights.count)
	{
		reset_light(&light_tmp);
		light_from_parsed(vector_at(&parsed->lights, i), &light_tmp);
		if (vector_push_back(&cx->light_v, &light_tmp) != SUCCESS)
			return (perr(ERR_MALLOC, PERR_M_VEC_PUSH));
		i++;
	}
	return (SUCCESS);
}
