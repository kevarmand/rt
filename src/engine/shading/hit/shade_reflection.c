/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shade_reflection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 13:22:44 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/18 18:55:42 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include <stdio.h>

static t_ray	build_reflection_ray(const t_hit *hit)
{
	t_vec3f	in_dir;
	t_vec3f	refl_dir;
	t_vec3f	tmp;
	float	ndotv;

	in_dir = vec3f_scale(hit->view_dir, -1.0f);
	ndotv = vec3f_dot(hit->normal, in_dir);
	tmp = vec3f_scale(hit->normal, 2.0f * ndotv);
	refl_dir = vec3f_sub(in_dir, tmp);
	refl_dir = vec3f_normalize(refl_dir);
	return (ray_finalize(vec3f_add(hit->point,
				vec3f_scale(hit->normal, 0.01f)), refl_dir));
}

t_vec3f	shade_reflection(const t_scene *scene,
			const t_hit *hit,
			t_shading_ctx *ctx,
			float factor)
{
	t_shading_ctx	child;
	t_ray			refl_ray;
	t_vec3f			refl_color;

	child = *ctx;
	child.depth++;
	child.contribution = ctx->contribution * factor;
	if (child.depth >= MAX_RECURSION_DEPTH
		|| child.contribution < MIN_CONTRIBUTION)
		return ((t_vec3f){0.0f, 0.0f, 0.0f});
	refl_ray = build_reflection_ray(hit);
	shading_ray(scene, &refl_ray, &child, &refl_color);
	return (vec3f_scale(refl_color, factor));
}
