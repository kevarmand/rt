/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   geometry_uv_prim.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 19:20:53 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/19 17:42:29 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../geometry.h"

static float	clamp_unit(float v)
{
	if (v > 1.0f)
		return (1.0f);
	if (v < -1.0f)
		return (-1.0f);
	return (v);
}

void	sphere_build_uv(const t_sphere *sphere, t_hit *hit)
{
	t_vec3f	local;
	float	phi;
	float	theta;

	local = vec3f_normalize(vec3f_sub(hit->point, sphere->center));
	phi = atan2f(local.z, local.x);
	if (phi < 0.0f)
		phi += 2.0f * (float)M_PI;
	theta = acosf(clamp_unit(local.y));
	hit->u = phi / (2.0f * (float)M_PI);
	hit->v = theta / (float)M_PI;
}

void	cylinder_build_uv(const t_cylinder *cylinder, t_hit *hit)
{
	t_vec3f	base_to_p;
	float	h;

	base_to_p = vec3f_sub(hit->point, cylinder->base);
	h = vec3f_dot(base_to_p, cylinder->axis);
	hit->u = atan2f(base_to_p.z, base_to_p.x) / (2.0f * (float)M_PI);
	if (hit->u < 0.0f)
		hit->u += 1.0f;
	hit->v = h / cylinder->height;
}

void	triangle_build_uv_bary(const t_surface *surface,
			float u, float v, t_hit *hit)
{
	float	w;

	w = 1.0f - u - v;
	hit->u = surface->map_uv[0] * w
		+ surface->map_uv[2] * u
		+ surface->map_uv[4] * v;
	hit->v = surface->map_uv[1] * w
		+ surface->map_uv[3] * u
		+ surface->map_uv[5] * v;
}

void	plane_build_uv(const t_surface *surface, t_hit *hit)
{
	hit->u = vec3f_dot(hit->point, surface->tang);
	hit->v = vec3f_dot(hit->point, surface->bitang);
	surface_apply_uv(surface, hit);
}
