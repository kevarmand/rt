/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shade_reflection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 13:22:44 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/10 16:30:17 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include <stdio.h>
static int	build_reflection_ray(const t_hit *hit,
			t_ray *refl_ray, t_vec3f *refl_dir)
{
	t_vec3f	in_dir;
	float	ndotv;
	t_vec3f	tmp;

	in_dir = vec3f_scale(hit->view_dir, -1.0f);
	ndotv = vec3f_dot(hit->normal, in_dir);
	tmp = vec3f_scale(hit->normal, 2.0f * ndotv);
	*refl_dir = vec3f_sub(in_dir, tmp);
	*refl_dir = vec3f_normalize(*refl_dir);
	*refl_ray = build_ray(vec3f_add(hit->point,
				vec3f_scale(hit->normal, 1e-2f)), *refl_dir);
	return (1);
}


void	shade_reflection(const t_scene *scene,
			const t_hit *hit, t_shading_ctx *ctx, t_vec3f *color)
{
	const t_material	*material;
	float				kr;
	t_shading_ctx		child;
	t_ray				refl_ray;
	t_vec3f				refl_dir;
	t_vec3f				refl_color;


	if (ctx->depth >= MAX_RECURSION_DEPTH || ctx->contribution < MIN_CONTRIBUTION)
		return ;
	if (!build_reflection_ray(hit, &refl_ray, &refl_dir))
		return ;
	child = *ctx;
	shading_ray(scene, &refl_ray, &child, color);
}
