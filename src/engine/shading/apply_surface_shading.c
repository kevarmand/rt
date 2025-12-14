/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_surface_shading.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 17:16:59 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/14 16:29:27 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include "scene.h"
#include <math.h>


static float	wrap01(float x)
{
	x = x - floorf(x);
	if (x < 0.0f)
		x += 1.0f;
	return (x);
}

static t_vec3f	vec3f_lerp2(t_vec3f c00, t_vec3f c10,
			t_vec3f c01, t_vec3f c11, float fx, float fy)
{
	t_vec3f	a;
	t_vec3f	b;
	t_vec3f	out;

	a = vec3f_add(vec3f_scale(c00, 1.0f - fx), vec3f_scale(c10, fx));
	b = vec3f_add(vec3f_scale(c01, 1.0f - fx), vec3f_scale(c11, fx));
	out = vec3f_add(vec3f_scale(a, 1.0f - fy), vec3f_scale(b, fy));
	return (out);
}


static t_vec3f	fetch_texel(const t_texture *t, int x, int y)
{
	int		index;
	t_vec3f	*px;

	index = y * t->width + x;
	px = (t_vec3f *)t->pixels;
	return (px[index]);
}



t_vec3f	sample_texture(const t_texture *t, float u, float v)
{
	float		x;
	float		y;
	int			x0;
	int			x1;
	int			y0;
	int			y1;
	float		fx;
	float		fy;
	t_vec3f		c00;
	t_vec3f		c10;
	t_vec3f		c01;
	t_vec3f		c11;

	u = wrap01(u);
	v = wrap01(v);
	x = u * (float)t->width - 0.5f;
	y = (1.0f - v) * (float)t->height - 0.5f;
	x0 = (int)floorf(x);
	y0 = (int)floorf(y);
	x1 = x0 + 1;
	y1 = y0 + 1;
	x0 = (x0 % t->width + t->width) % t->width;
	y0 = (y0 % t->height + t->height) % t->height;
	x1 = (x1 % t->width + t->width) % t->width;
	y1 = (y1 % t->height + t->height) % t->height;
	fx = x - floorf(x);
	fy = y - floorf(y);
	c00 = fetch_texel(t, x0, y0);
	c10 = fetch_texel(t, x1, y0);
	c01 = fetch_texel(t, x0, y1);
	c11 = fetch_texel(t, x1, y1);
	return (vec3f_lerp2(c00, c10, c01, c11, fx, fy));
}

static int	is_checker_odd(float u, float v)
{
	int	u_i;
	int	v_i;

	u_i = (int)floorf(u);
	v_i = (int)floorf(v);
	return ((u_i + v_i) & 1);
}

static t_vec3f	get_surface_color(const t_scene *sc,
				const t_surface_map *surf,
				t_index tex_id,
				float u,
				float v)
{
	t_texture	*tex;

	if (tex_id == SCENE_ID_NONE)
		return (surf->color);
	tex = &sc->textures[tex_id];
	if (tex->width <= 0 || tex->height <= 0 || !tex->pixels)
		return (surf->color);
	return (sample_texture(tex, u, v));
}


void	apply_surface_shading(const t_scene *sc, t_hit *hit)
{
	t_surface_map	*surf;
	int				odd;

	surf = &sc->surfaces[hit->surface_id];
	if (surf->checker_mode == 0)
	{
		hit->albedo = get_surface_color(sc, surf,
				surf->texture_albedo_id, hit->u, hit->v);
		return ;
	}
	odd = is_checker_odd(hit->u, hit->v);
	if (odd)
	{
		if (surf->checker_mode == 3)
			hit->albedo = get_surface_color(sc, surf,
					surf->checker_texture_id, hit->u, hit->v);
		else
			hit->albedo = surf->checker_color;
	}
	else
	{
		hit->albedo = get_surface_color(sc, surf,
				surf->texture_albedo_id, hit->u, hit->v);
	}
}

