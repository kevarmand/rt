/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_surface_uv.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 16:31:16 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/02 18:11:08 by kearmand         ###   ########.fr       */
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

// void	init_surface_uv_sphere(t_scene *sc, t_surface *s, t_primitive *p)
// {
// 	t_sphere	*sp;
// 	t_texture	*tex;
	
// }

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
	// i = 0;
	// while (i < sc->primitive_count)
	// {
	// 	p = &sc->primitives[i];
	// 	s = &sc->surfaces[p->surface_id];
	// 	if (p->type == PRIM_SPHERE)
	// 		init_surface_uv_sphere(sc, s, p);
	// 	i++;
	// }
}
