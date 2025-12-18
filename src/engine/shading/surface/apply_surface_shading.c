/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_surface_shading.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 17:16:59 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/18 18:53:03 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include "scene.h"
#include <math.h>
#include "../shading.h"

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
				t_hit *hit)
{
	t_texture	*tex;

	if (tex_id == SCENE_ID_NONE)
		return (surf->color);
	tex = &sc->textures[tex_id];
	if (tex->width <= 0 || tex->height <= 0 || !tex->pixels)
		return (surf->color);
	return (sample_texture(tex, hit->u, hit->v));
}

void	apply_surface_shading(const t_scene *sc, t_hit *hit)
{
	t_surface_map	*surf;
	int				odd;

	surf = &sc->surfaces[hit->surface_id];
	if (surf->checker_mode == 0)
	{
		hit->albedo = get_surface_color(sc, surf,
				surf->texture_albedo_id, hit);
		return ;
	}
	odd = is_checker_odd(hit->u, hit->v);
	if (odd)
	{
		if (surf->checker_mode == 3)
			hit->albedo = get_surface_color(sc, surf,
					surf->checker_texture_id, hit);
		else
			hit->albedo = surf->checker_color;
	}
	else
	{
		hit->albedo = get_surface_color(sc, surf,
				surf->texture_albedo_id, hit);
	}
}
