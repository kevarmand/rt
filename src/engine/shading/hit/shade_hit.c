/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shade_hit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 21:21:27 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 01:17:57 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include "scene.h"
#include <math.h>
#include "../shading.h"

static void	compute_secondary_weights(const t_material *mat,
				const t_hit *hit, const t_shading_ctx *ctx,
				float weights[2])
{
	float	fresnel;

	if (mat->reflection <= 0.0f)
	{
		weights[0] = 0.0f;
		weights[1] = mat->refraction;
		return ;
	}
	if (mat->refraction <= 0.0f)
	{
		weights[0] = mat->reflection;
		weights[1] = 0.0f;
		return ;
	}
	fresnel = fresnel_schlick(mat, hit, ctx);
	weights[0] = mat->reflection * fresnel;
	weights[1] = mat->refraction * (1.0f - fresnel);
}

int	shade_hit(const t_scene *scene, const t_hit *hit,
		t_shading_ctx *ctx, t_vec3f *color_out)
{
	const t_material	*material;
	t_vec3f				direct_color;
	float				weights[2];

	direct_color = (t_vec3f){0.0f, 0.0f, 0.0f};
	shade_ambient(scene, hit, &direct_color);
	shade_direct_lights(scene, hit, &direct_color);
	material = &scene->materials[hit->material_id];
	compute_secondary_weights(material, hit, ctx, weights);
	if (weights[0] > 0.0f)
		direct_color = vec3f_add(direct_color,
				shade_reflection(scene, hit, ctx, weights[0]));
	if (weights[1] > 0.0f)
		direct_color = vec3f_add(direct_color,
				shade_refraction(scene, hit, ctx, weights[1]));
	*color_out = direct_color;
	return (0);
}
