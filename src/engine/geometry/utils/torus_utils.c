/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   torus_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 20:01:01 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 01:12:11 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../geometry.h"
#include <math.h>

static float	wrap01(float x)
{
	x = x - floorf(x);
	if (x < 0.0f)
		x += 1.0f;
	return (x);
}

t_vec3f	torus_compute_radial(const t_torus *torus, t_vec3f p)
{
	t_vec3f	axis;
	t_vec3f	proj;
	float	len2;
	t_vec3f	ref;

	axis = torus->normal;
	proj = vec3f_sub(p, vec3f_scale(axis, vec3f_dot(p, axis)));
	len2 = vec3f_dot(proj, proj);
	if (len2 < 0.0000000001f)
	{
		ref = (t_vec3f){0.0f, 1.0f, 0.0f};
		if (fabsf(axis.y) > 0.999f)
			ref = (t_vec3f){1.0f, 0.0f, 0.0f};
		return (vec3f_normalize(vec3f_cross(ref, axis)));
	}
	return (vec3f_scale(proj, 1.0f / sqrtf(len2)));
}

void	geo_build_frame(t_vec3f axis, t_vec3f *t0, t_vec3f *b0)
{
	t_vec3f	ref;

	ref = (t_vec3f){0.0f, 1.0f, 0.0f};
	if (fabsf(axis.y) > 0.999f)
		ref = (t_vec3f){1.0f, 0.0f, 0.0f};
	*t0 = vec3f_normalize(vec3f_cross(ref, axis));
	*b0 = vec3f_normalize(vec3f_cross(axis, *t0));
}

void	torus_build_uv(const t_torus *torus, t_vec3f p, t_vec3f radial,
				t_hit *hit)
{
	t_vec3f	t0;
	t_vec3f	b0;
	t_vec3f	o;
	t_vec3f	q;
	float	theta;

	geo_build_frame(torus->normal, &t0, &b0);
	theta = atan2f(vec3f_dot(radial, b0), vec3f_dot(radial, t0));
	o = vec3f_scale(radial, torus->r0);
	q = vec3f_sub(p, o);
	hit->u = wrap01(theta * (1.0f / (2.0f * (float)M_PI)));
	hit->v = wrap01(atan2f(vec3f_dot(q, torus->normal), vec3f_dot(q, radial))
			* (1.0f / (2.0f * (float)M_PI)));
}

void	torus_set_tbn(const t_torus *torus, t_vec3f radial, t_vec3f normal,
			t_bump_apply *bump)
{
	bump->tangent = vec3f_normalize(vec3f_cross(torus->normal, radial));
	bump->bitangent = vec3f_normalize(vec3f_cross(normal, bump->tangent));
}
