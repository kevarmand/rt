/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shade_ambiant.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 15:00:07 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/02 11:13:45 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include "scene.h"
#include "vector.h"

void	shade_ambient(const t_scene *scene,
			const t_hit *hit, t_vec3f *color_out)
{
	const t_surface		*surface;
	t_vec3f				ambiant_color;
	t_vec3f				tmp;

	surface = &scene->surfaces[hit->surface_id];

	/* surface->color = albedo (0..1)
	   material->ambient = ka
	   scene->ambient_color = intensité ambiante globale */
	tmp = vec3f_scale(hit->albedo, 0.2f); //on peut ajuster l'intensité ambiante ici si besoin
	ambiant_color = vec3f_mul(tmp, scene->ambient_color);

	*color_out = vec3f_add(*color_out, ambiant_color);
}

