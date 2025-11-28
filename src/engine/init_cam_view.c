/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_cam_view.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 15:30:33 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/27 21:33:26 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include "scene.h"
#include <math.h>
#include "vector.h"

void	init_cam_views(t_scene *scene)
{
	int		index;
	t_vec3f	forward;
	t_vec3f	up;
	t_vec3f	tmp;

	index = 0;
	while (index < scene->camera_count)
	{
		forward = scene->cameras[index].forward;
		up = (t_vec3f){0.0f, 1.0f, 0.0f};
		if (fabsf(vec3f_dot(forward, up)) > 0.999f)
			up = (t_vec3f){0.0f, 0.0f, 1.0f};
		tmp = vec3f_cross(forward, up);
		scene->cameras[index].right = vec3f_normalize(tmp);
		index++;
	}
}
