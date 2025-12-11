/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shade_hit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 21:21:27 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/11 22:59:29 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include "scene.h"
#include <math.h>
float	fresnel_eval(const t_material *mat,
			const t_hit *hit, const t_shading_ctx *ctx)
{
	t_vec3f	normal;
	float	n1;
	float	n2;
	float	ratio;
	float	cos_i;
	float	sin2_t;
	float	cos_t;
	float	r_te;
	float	r_tm;

	normal = hit->normal;
	if (vec3f_dot(normal, hit->view_dir) > 0.0f)
		normal = vec3f_scale(normal, -1.0f);
	n1 = ctx->current_ior;
	if (n1 == 1.0f)
		n2 = mat->ior;
	else
		n2 = 1.0f;
	ratio = n1 / n2;
	cos_i = -vec3f_dot(normal, hit->view_dir);
	if (cos_i > 1.0f)
		cos_i = 1.0f;
	sin2_t = ratio * ratio * (1.0f - cos_i * cos_i);
	if (sin2_t > 1.0f)
		return (1.0f);
	cos_t = sqrtf(1.0f - sin2_t);
	r_te = (n1 * cos_i - n2 * cos_t) / (n1 * cos_i + n2 * cos_t);
	r_tm = (n2 * cos_i - n1 * cos_t) / (n2 * cos_i + n1 * cos_t);
	return (0.5f * (r_te * r_te + r_tm * r_tm));
}

float	fresnel_schlick(const t_material *mat,
			const t_hit *hit, const t_shading_ctx *ctx)
{
	t_vec3f	normal;
	t_vec3f	in_dir;
	float	n1;
	float	n2;
	float	cos_i;
	float	f0;
	float	x;

	in_dir = vec3f_scale(hit->view_dir, -1.0f);
	normal = hit->normal;
	if (vec3f_dot(normal, in_dir) < 0.0f)
		normal = vec3f_scale(normal, -1.0f);
	n1 = ctx->current_ior;
	if (n1 == 1.0f)
		n2 = mat->ior;
	else
		n2 = 1.0f;
	cos_i = vec3f_dot(normal, in_dir);
	if (cos_i < 0.0f)
		cos_i = 0.0f;
	if (cos_i > 1.0f)
		cos_i = 1.0f;
	f0 = (n1 - n2) / (n1 + n2);
	f0 = f0 * f0;
	x = 1.0f - cos_i;
	return (f0 + (1.0f - f0)
		* x * x * x * x * x);
}


// int	shade_hit(const t_scene *scene, const t_hit *hit,
// 		t_shading_ctx *ctx, t_vec3f *color_out)
// {
// 	const t_material	*mat;
// 	t_vec3f				local;
// 	t_vec3f				refl;
// 	t_vec3f				refr;
// 	float				r;
// 	float				t;
// 	float				k;

// 	local = (t_vec3f){0.0f, 0.0f, 0.0f};
// 	shade_ambient(scene, hit, &local);
// 	shade_direct_lights(scene, hit, &local);
// 	refl = (t_vec3f){0.0f, 0.0f, 0.0f};
// 	refr = (t_vec3f){0.0f, 0.0f, 0.0f};
// 	mat = &scene->materials[hit->material_id];
// 	if (mat->reflection > 0.0f || mat->refraction > 0.0f)
// 	{
// 		printf("Here\n");
// 		if (mat->reflection > 0.0f)
// 			shade_reflection(scene, hit, ctx, &refl);
// 		if (mat->refraction > 0.0f)
// 			shade_refraction(scene, hit, ctx, &refr);
// 		r = fresnel_schlick(mat, hit, ctx);
// 		t = 1.0f - r;
// 		k = 1.0f - r * mat->reflection - t * mat->refraction;
// 		local = vec3f_scale(local, k);
// 		refl = vec3f_scale(refl, r * mat->reflection);
// 		refr = vec3f_scale(refr, t * mat->refraction);
// 	}
// 	*color_out = vec3f_add(local, vec3f_add(refl, refr));
// 	return (0);
// }
int	shade_hit(const t_scene *scene, const t_hit *hit,
		t_shading_ctx *ctx, t_vec3f *color_out)
{
	const t_material	*material;
	t_vec3f			local_color;
	t_vec3f			reflection_color;
	t_vec3f			refraction_color;
	float			fresnel_factor;
	float			reflect_factor;
	float			transmit_factor;
	float			local_factor;
	float			total_factor;
	t_shading_ctx		reflection_ctx;
	t_shading_ctx		refraction_ctx;

	local_color = (t_vec3f){0.0f, 0.0f, 0.0f};
	reflection_color = (t_vec3f){0.0f, 0.0f, 0.0f};
	refraction_color = (t_vec3f){0.0f, 0.0f, 0.0f};
	shade_ambient(scene, hit, &local_color);
	shade_direct_lights(scene, hit, &local_color);
	// *color_out = local_color;
	// return (0);
	material = &scene->materials[hit->material_id];
	fresnel_factor = fresnel_schlick(material, hit, ctx);
	reflect_factor = fresnel_factor * material->reflection;
	transmit_factor = (1.0f - fresnel_factor)
		* material->refraction;
	if (reflect_factor < 0.0f)
		reflect_factor = 0.0f;
	if (transmit_factor < 0.0f)
		transmit_factor = 0.0f;
	total_factor = reflect_factor + transmit_factor;
	if (total_factor > 1.0f)
	{
		reflect_factor /= total_factor;
		transmit_factor /= total_factor;
	}
	local_factor = 1.0f - reflect_factor - transmit_factor;
	if (local_factor < 0.0f)
		local_factor = 0.0f;
	local_color = vec3f_scale(local_color, local_factor);
	if (reflect_factor > 0.0f && material->reflection > 0.0f)
	{
		reflection_ctx = *ctx;
		reflection_ctx.depth++;
		reflection_ctx.contribution = ctx->contribution
			* reflect_factor;
		shade_reflection(scene, hit, &reflection_ctx,
			&reflection_color);
	}
	if (transmit_factor > 0.0f && material->refraction > 0.0f)
	{
		refraction_ctx = *ctx;
		refraction_ctx.depth++;
		refraction_ctx.contribution = ctx->contribution
			* transmit_factor;
		shade_refraction(scene, hit, &refraction_ctx,
			&refraction_color);
	}
	*color_out = vec3f_add(local_color,
			vec3f_add(vec3f_scale(reflection_color, reflect_factor),
				vec3f_scale(refraction_color, transmit_factor)));
	return (0);
}

