/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fresnel_schlick.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 18:44:33 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/18 18:49:16 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include "../shading.h"
#include <math.h>

typedef struct s_fresnel_ctx
{
	t_vec3f	normal;
	t_vec3f	in_dir;
	float	n1;
	float	n2;
	float	cos_i;
	float	f0;
	float	x;
}	t_fresnel_ctx;

float	fresnel_schlick(const t_material *mat,
			const t_hit *hit, const t_shading_ctx *ctx)
{
	t_fresnel_ctx	l;

	l.in_dir = vec3f_scale(hit->view_dir, -1.0f);
	l.normal = hit->normal;
	if (vec3f_dot(l.normal, l.in_dir) < 0.0f)
		l.normal = vec3f_scale(l.normal, -1.0f);
	l.n1 = ctx->current_ior;
	if (l.n1 == 1.0f)
		l.n2 = mat->ior;
	else
		l.n2 = 1.0f;
	l.cos_i = vec3f_dot(l.normal, l.in_dir);
	if (l.cos_i < 0.0f)
		l.cos_i = 0.0f;
	if (l.cos_i > 1.0f)
		l.cos_i = 1.0f;
	l.f0 = (l.n1 - l.n2) / (l.n1 + l.n2);
	l.f0 = l.f0 * l.f0;
	l.x = 1.0f - l.cos_i;
	return (l.f0 + (1.0f - l.f0)
		* l.x * l.x * l.x * l.x * l.x);
}

// float	fresnel_eval(const t_material *mat,
// 			const t_hit *hit, const t_shading_ctx *ctx)
// {
// 	t_vec3f	normal;
// 	float	n1;
// 	float	n2;
// 	float	ratio;
// 	float	cos_i;
// 	float	sin2_t;
// 	float	cos_t;
// 	float	r_te;
// 	float	r_tm;

// 	normal = hit->normal;
// 	if (vec3f_dot(normal, hit->view_dir) > 0.0f)
// 		normal = vec3f_scale(normal, -1.0f);
// 	n1 = ctx->current_ior;
// 	if (n1 == 1.0f)
// 		n2 = mat->ior;
// 	else
// 		n2 = 1.0f;
// 	ratio = n1 / n2;
// 	cos_i = -vec3f_dot(normal, hit->view_dir);
// 	if (cos_i > 1.0f)
// 		cos_i = 1.0f;
// 	sin2_t = ratio * ratio * (1.0f - cos_i * cos_i);
// 	if (sin2_t > 1.0f)
// 		return (1.0f);
// 	cos_t = sqrtf(1.0f - sin2_t);
// 	r_te = (n1 * cos_i - n2 * cos_t) / (n1 * cos_i + n2 * cos_t);
// 	r_tm = (n2 * cos_i - n1 * cos_t) / (n2 * cos_i + n1 * cos_t);
// 	return (0.5f * (r_te * r_te + r_tm * r_tm));
// }
