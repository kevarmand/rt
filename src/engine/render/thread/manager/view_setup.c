/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   view_setup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 11:05:22 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/28 16:25:38 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include "vector.h"
#include <math.h>

void	view_setup(t_render_view *view, int width, int height)
{
	float	aspect_ratio;
	float	fov_rad;
	float	half_w;
	float	half_h;
	t_vec3f	tmp_right;
	t_vec3f	tmp_up;
	t_vec3f	forward_step;

	aspect_ratio = (float)width / (float)height;
	fov_rad = view->fov_deg * (float)M_PI / 180.0f;
	half_w = tanf(fov_rad * 0.5f);
	half_h = half_w / aspect_ratio;

	// On s'assure que la base est propre
	view->forward = vec3f_normalize(view->forward);
	view->right = vec3f_normalize(view->right);
	view->up = vec3f_normalize(vec3f_cross(view->right, view->forward));

	tmp_right = vec3f_scale(view->right, -half_w);
	tmp_up = vec3f_scale(view->up, half_h);
	forward_step = view->forward; // distance focale = 1.0

	// ⚠️ Ici la correction : le plan image est DEVANT la caméra
	view->p0 = vec3f_add(
					vec3f_add(view->origin, forward_step),
					vec3f_add(tmp_right, tmp_up));

	view->dx = vec3f_scale(view->right, (2.0f * half_w) / (float)width);
	view->dy = vec3f_scale(view->up, (-2.0f * half_h) / (float)height);
}
