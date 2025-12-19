/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   geometry_uv.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 19:18:44 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/19 17:42:29 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../geometry.h"

void	surface_apply_uv(const t_surface *surface, t_hit *hit)
{
	float	u;
	float	v;

	u = hit->u;
	v = hit->v;
	hit->u = surface->map_uv[0] * u
		+ surface->map_uv[1] * v
		+ surface->map_uv[4];
	hit->v = surface->map_uv[2] * u
		+ surface->map_uv[3] * v
		+ surface->map_uv[5];
}
