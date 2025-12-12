/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_surface_shading.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 17:16:59 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/12 18:19:16 by kearmand         ###   ########.fr       */
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



static t_vec3f	sample_texture(const t_texture *t, float u, float v)
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

void	apply_surface_shading(const t_scene *sc, t_hit *hit)
{
	t_material	*mat;
	t_texture	*tex;
	t_surface	*surf;
	t_vec3f		c;

	mat = &sc->materials[hit->material_id];
	tex = 0;
	if (mat->texture_albedo_id >= 0)
		tex = &sc->textures[mat->texture_albedo_id];
	if (!tex || tex->width <= 0 || tex->height <= 0 || !tex->pixels)
	{
		surf = &sc->surfaces[hit->surface_id];
		hit->albedo = surf->color;
		return ;
	}
	
	c = sample_texture(tex, hit->u, hit->v);
	hit->albedo = c;
}
