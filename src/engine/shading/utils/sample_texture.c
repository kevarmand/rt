/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sample_texture.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 17:29:23 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/18 18:33:46 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scene.h"
#include "vector.h"
#include <math.h>

static float	wrap01(float x)
{
	x = x - floorf(x);
	if (x < 0.0f)
		x += 1.0f;
	return (x);
}

static int	wrap_index(int value, int size)
{
	value = value % size;
	if (value < 0)
		value += size;
	return (value);
}

static t_vec3f	fetch_texel(const t_texture *tex, int x, int y)
{
	int		index;
	t_vec3f	*px;

	index = y * tex->width + x;
	px = (t_vec3f *)tex->pixels;
	return (px[index]);
}

typedef struct s_tex_sample
{
	int		x0;
	int		x1;
	int		y0;
	int		y1;
	float	fx;
	float	fy;
}	t_tex_sample;

static void	tex_compute_sample(const t_texture *tex, float u, float v,
				t_tex_sample *s)
{
	float	x;
	float	y;
	int		x0;
	int		y0;

	u = wrap01(u);
	v = wrap01(v);
	x = u * (float)tex->width - 0.5f;
	y = (1.0f - v) * (float)tex->height - 0.5f;
	x0 = (int)floorf(x);
	y0 = (int)floorf(y);
	s->fx = x - floorf(x);
	s->fy = y - floorf(y);
	s->x0 = wrap_index(x0, tex->width);
	s->y0 = wrap_index(y0, tex->height);
	s->x1 = wrap_index(x0 + 1, tex->width);
	s->y1 = wrap_index(y0 + 1, tex->height);
}

t_vec3f	sample_texture(const t_texture *tex, float u, float v)
{
	t_tex_sample	s;
	t_vec3f			a;
	t_vec3f			b;

	tex_compute_sample(tex, u, v, &s);
	a = vec3f_lerp(fetch_texel(tex, s.x0, s.y0),
			fetch_texel(tex, s.x1, s.y0), s.fx);
	b = vec3f_lerp(fetch_texel(tex, s.x0, s.y1),
			fetch_texel(tex, s.x1, s.y1), s.fx);
	return (vec3f_lerp(a, b, s.fy));
}
