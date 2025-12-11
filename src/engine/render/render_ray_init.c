/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_ray_init.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 13:41:08 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/11 11:20:06 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include "vector.h"
#include "engine.h"

void	build_ray_for_pixel(const t_render_view *view,
			float img_x, float img_y, t_ray *ray)
{
	t_vec3f	offset_x;
	t_vec3f	offset_y;
	t_vec3f	pixel_pos;
	t_vec3f	dir;

	offset_x = vec3f_scale(view->dx, img_x);
	offset_y = vec3f_scale(view->dy, img_y);
	pixel_pos = vec3f_add(view->p0, offset_x);
	pixel_pos = vec3f_add(pixel_pos, offset_y);
	dir = vec3f_sub(pixel_pos, view->origin);
	dir = vec3f_normalize(dir);
	ray->origin = view->origin;
	ray->dir = dir;
}
