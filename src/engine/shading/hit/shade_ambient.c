/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shade_ambient.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 15:00:07 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 03:23:20 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include "scene.h"
#include "vector.h"

void	shade_ambient(const t_scene *scene,
			const t_hit *hit, t_vec3f *color_out)
{
	const t_material		*material;
	t_vec3f					ambient_color;
	t_vec3f					tmp;

	material = &scene->materials[hit->material_id];
	tmp = vec3f_scale(hit->albedo, material->ambient);
	ambient_color = vec3f_mul(tmp, scene->ambient_color);
	*color_out = vec3f_add(*color_out, ambient_color);
}
