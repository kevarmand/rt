/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_surface_uv.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 16:31:16 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/02 19:04:29 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scene.h"
#include "vector.h"
#include <math.h>

static t_texture	*get_albedo_tex(t_scene *sc, t_primitive *p)
{
	t_material	*m;
	int			tid;

	m = &sc->materials[p->material_id];
	tid = m->texture_albedo_id;
	if (tid < 0)
		return (0);
	return (&sc->textures[tid]);
}

static void	init_uv_mode0(t_surface *s, t_plane *pl)
{
	t_vec3f	u;
	t_vec3f	v;

	if (fabsf(pl->normal[0]) < 0.9f)
		u = (t_vec3f){1.0f, 0.0f, 0.0f};
	else
		u = (t_vec3f){0.0f, 1.0f, 0.0f};
	u = proj_on_plane(u, pl->normal);
	u = vec3f_normalize(u);
	v = vec3f_cross(pl->normal, u);
	v = vec3f_normalize(v);
	s->map_uv[0] = u[0];
	s->map_uv[1] = u[1];
	s->map_uv[2] = u[2];
	s->map_uv[3] = v[0];
	s->map_uv[4] = v[1];
	s->map_uv[5] = v[2];
	s->scale_u = 1.0f;
	s->scale_v = 0.0f;
}

static void	init_uv_mode1(t_surface *s, t_plane *pl)
{
	t_vec3f	u;
	t_vec3f	v;
	float	len;

	u = (t_vec3f){s->map_uv[0], s->map_uv[1], s->map_uv[2]};
	u = proj_on_plane(u, pl->normal);
	len = vec3f_length(u);
	if (len < 1e-6f)
		return (init_uv_mode0(s, pl));
	u = vec3f_scale(u, 1.0f / len);
	v = vec3f_cross(pl->normal, u);
	v = vec3f_normalize(v);
	s->map_uv[0] = u[0];
	s->map_uv[1] = u[1];
	s->map_uv[2] = u[2];
	s->map_uv[3] = v[0];
	s->map_uv[4] = v[1];
	s->map_uv[5] = v[2];
	s->scale_u = len;
	s->scale_v = 0.0f;
}

static void	init_uv_mode2(t_surface *s, t_plane *pl)
{
	t_vec3f	u;
	t_vec3f	v;
	float	lu;
	float	lv;

	u = (t_vec3f){s->map_uv[0], s->map_uv[1], s->map_uv[2]};
	v = (t_vec3f){s->map_uv[3], s->map_uv[4], s->map_uv[5]};
	u = proj_on_plane(u, pl->normal);
	v = proj_on_plane(v, pl->normal);
	lu = vec3f_length(u);
	if (lu < 1e-6f)
		return (init_uv_mode0(s, pl));
	lv = vec3f_length(v);
	if (lv < 1e-6f)
		return (init_uv_mode1(s, pl));
	u = vec3f_scale(u, 1.0f / lu);
	v = vec3f_scale(v, 1.0f / lv);
	s->map_uv[0] = u[0];
	s->map_uv[1] = u[1];
	s->map_uv[2] = u[2];
	s->map_uv[3] = v[0];
	s->map_uv[4] = v[1];
	s->map_uv[5] = v[2];
	s->scale_u = lu;
	s->scale_v = lv;
}

static void	apply_texture_ratio(t_surface *s, t_texture *t)
{
	float	r;

	if (!t || t->width == 0 || t->height == 0)
	{
		s->scale_v = s->scale_u;
		return ;
	}
	r = (float)t->height / (float)t->width;
	s->scale_v = s->scale_u * r;
}

static void	init_surface_uv_plane(t_scene *sc, t_surface *s, t_primitive *p)
{
	t_plane		*pl;
	t_texture	*tex;

	pl = &p->pl;
	s->normal = pl->normal;
	if (s->uv_mod == 0)
		init_uv_mode0(s, pl);
	else if (s->uv_mod == 1)
		init_uv_mode1(s, pl);
	else
		init_uv_mode2(s, pl);
	if (s->scale_v == 0.0f)
	{
		tex = get_albedo_tex(sc, p);
		apply_texture_ratio(s, tex);
	}
}

static void	sphere_dir_to_uv(t_vec3f direction,
				float *u_out, float *v_out)
{
	t_vec3f	normalized_dir;
	float	theta_angle;
	float	phi_angle;
	float	u_value;

	normalized_dir = vec3f_normalize(direction);
	theta_angle = atan2f(normalized_dir[2], normalized_dir[0]);
	phi_angle = acosf(normalized_dir[1]);
	u_value = 0.5f + theta_angle / (2.0f * (float)M_PI);
	if (u_value < 0.0f)
		u_value += 1.0f;
	else if (u_value >= 1.0f)
		u_value -= 1.0f;
	*u_out = u_value;
	*v_out = phi_angle / (float)M_PI;
}


static void	init_surface_uv_sphere_mode1(t_surface *surface)
{
	t_vec3f	anchor_vector;
	float	anchor_u;
	float	anchor_v;

	anchor_vector = (t_vec3f){surface->map_uv[0],
		surface->map_uv[1], surface->map_uv[2]};
	sphere_dir_to_uv(anchor_vector, &anchor_u, &anchor_v);
	surface->map_uv[0] = 1.0f;
	surface->map_uv[1] = 0.0f;
	surface->map_uv[2] = 0.0f;
	surface->map_uv[3] = 1.0f;
	surface->map_uv[4] = 0.5f - anchor_u;
	surface->map_uv[5] = 0.5f - anchor_v;
}

static float	wrap_u_relative(float value, float reference)
{
	float	diff;

	diff = value - reference;
	if (diff > 0.5f)
		value -= 1.0f;
	else if (diff < -0.5f)
		value += 1.0f;
	return (value);
}



static void	init_surface_uv_sphere_mode2(t_surface *surface)
{
	t_vec3f	center_vector;
	t_vec3f	up_vector;
	float	center_u;
	float	center_v;
	float	up_u;
	float	up_v;
	float	adjusted_up_u;
	float	delta_u;
	float	delta_v;
	float	source_length;
	float	unit_src_u;
	float	unit_src_v;
	float	cosine_angle;
	float	sine_angle;
	float	scale_factor;
	float	coef_uu;
	float	coef_uv;
	float	coef_vu;
	float	coef_vv;
	float	offset_u;
	float	offset_v;

	center_vector = (t_vec3f){surface->map_uv[0],
		surface->map_uv[1], surface->map_uv[2]};
	up_vector = (t_vec3f){surface->map_uv[3],
		surface->map_uv[4], surface->map_uv[5]};
	sphere_dir_to_uv(center_vector, &center_u, &center_v);
	sphere_dir_to_uv(up_vector, &up_u, &up_v);
	adjusted_up_u = wrap_u_relative(up_u, center_u);
	delta_u = adjusted_up_u - center_u;
	delta_v = up_v - center_v;
	source_length = sqrtf(delta_u * delta_u + delta_v * delta_v);
	if (source_length < 1e-6f)
	{
		init_surface_uv_sphere_mode1(surface);
		return ;
	}
	unit_src_u = delta_u / source_length;
	unit_src_v = delta_v / source_length;
	cosine_angle = unit_src_v;
	sine_angle = unit_src_u;
	scale_factor = 0.5f / source_length;
	coef_uu = scale_factor * cosine_angle;
	coef_uv = -scale_factor * sine_angle;
	coef_vu = scale_factor * sine_angle;
	coef_vv = scale_factor * cosine_angle;
	offset_u = 0.5f
		- (coef_uu * center_u + coef_uv * center_v);
	offset_v = 0.5f
		- (coef_vu * center_u + coef_vv * center_v);
	surface->map_uv[0] = coef_uu;
	surface->map_uv[1] = coef_uv;
	surface->map_uv[2] = coef_vu;
	surface->map_uv[3] = coef_vv;
	surface->map_uv[4] = offset_u;
	surface->map_uv[5] = offset_v;
}



void	init_surface_uv_sphere(t_scene *scene, t_surface *surface,
				t_primitive *primitive)
{
	(void)scene;
	(void)primitive;
	surface->normal = (t_vec3f){0.0f, 0.0f, 0.0f};
	if (surface->uv_mod == 0)
	{
		surface->map_uv[0] = 1.0f;
		surface->map_uv[1] = 0.0f;
		surface->map_uv[2] = 0.0f;
		surface->map_uv[3] = 1.0f;
		surface->map_uv[4] = 0.0f;
		surface->map_uv[5] = 0.0f;
	}
	else if (surface->uv_mod == 1)
		init_surface_uv_sphere_mode1(surface);
	else
		init_surface_uv_sphere_mode2(surface);
}



void	init_surface_uv(t_scene *sc)
{
	int			i;
	t_surface	*s;
	t_primitive	*p;

	i = 0;
	while (i < sc->plane_count)
	{
		p = &sc->planes[i];
		s = &sc->surfaces[p->surface_id];
		init_surface_uv_plane(sc, s, p);
		i++;
	}
	i = 0;
	while (i < sc->primitive_count)
	{
		p = &sc->primitives[i];
		s = &sc->surfaces[p->surface_id];
		if (p->type == PRIM_SPHERE)
			init_surface_uv_sphere(sc, s, p);
		i++;
	}
}
