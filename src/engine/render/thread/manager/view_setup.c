/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   view_setup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 11:05:22 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/17 23:14:05 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include "vector.h"
#include <math.h>

typedef struct s_view_plane
{
	float	half_w;
	float	half_h;
}	t_view_plane;

static void	view_compute_plane(float fov_deg, int width, int height,
				t_view_plane *plane)
{
	float	aspect_ratio;
	float	fov_rad;

	aspect_ratio = (float)width / (float)height;
	fov_rad = fov_deg * (float)M_PI / 180.0f;
	plane->half_w = tanf(fov_rad * 0.5f);
	plane->half_h = plane->half_w / aspect_ratio;
}

static void	view_orthonormalize_basis(t_render_view *view)
{
	view->forward = vec3f_normalize(view->forward);
	view->right = vec3f_normalize(view->right);
	view->up = vec3f_normalize(vec3f_cross(view->right, view->forward));
}

static void	view_build_rayspace(t_render_view *view, int width, int height,
				const t_view_plane *plane)
{
	t_vec3f	tmp_right;
	t_vec3f	tmp_up;

	tmp_right = vec3f_scale(view->right, -plane->half_w);
	tmp_up = vec3f_scale(view->up, plane->half_h);
	view->p0 = vec3f_add(vec3f_add(view->origin, view->forward),
			vec3f_add(tmp_right, tmp_up));
	view->dx = vec3f_scale(view->right,
			(2.0f * plane->half_w) / (float)width);
	view->dy = vec3f_scale(view->up,
			(-2.0f * plane->half_h) / (float)height);
}

void	view_setup(t_render_view *view, int width, int height)
{
	t_view_plane	plane;

	view_compute_plane(view->fov_deg, width, height, &plane);
	view_orthonormalize_basis(view);
	view_build_rayspace(view, width, height, &plane);
}
