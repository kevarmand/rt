/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   geometry_tbn.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 19:22:43 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/18 19:51:35 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../geometry.h"

void	sphere_build_tbn(t_vec3f normal,
			t_vec3f *tangent, t_vec3f *bitangent)
{
	t_vec3f	tmp;

	tmp = (t_vec3f){-normal.z, 0.0f, normal.x};
	if (fabsf(normal.y) > 0.999f)
		tmp = (t_vec3f){1.0f, 0.0f, 0.0f};
	*tangent = vec3f_normalize(tmp);
	*bitangent = vec3f_normalize(vec3f_cross(*tangent, normal));
}

void	cylinder_build_tbn(const t_cylinder *cylinder,
			t_vec3f normal,
			t_vec3f *tangent, t_vec3f *bitangent)
{
	*bitangent = vec3f_normalize(cylinder->axis);
	*tangent = vec3f_normalize(vec3f_cross(*bitangent, normal));
}
