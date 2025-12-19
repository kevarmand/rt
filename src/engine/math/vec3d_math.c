/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3d_math.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: norivier <norivier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 17:55:54 by norivier          #+#    #+#             */
/*   Updated: 2025/12/18 18:03:45 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"
#include "attributes.h"

FORCEINLINE
extern inline t_vec3d	vec3d_scale(t_vec3d a, double s)
{
	return (a * (t_vec3d){s, s, s, 0});
}

FORCEINLINE
extern inline float	vec3d_dot(t_vec3d a, t_vec3d b)
{
	t_vec3d	tmp;

	tmp = a * b;
	return (tmp.x + tmp.y + tmp.z);
}

FORCEINLINE
extern inline t_vec3d	vec3f_to_vec3d(t_vec3f vf)
{
	t_vec3d	vd;

	vd.x = (double)vf.x;
	vd.y = (double)vf.y;
	vd.z = (double)vf.z;
	vd.w = (double)0.0;
	return (vd);
}

FORCEINLINE
extern inline t_vec3f	vec3d_to_vec3f(t_vec3d vd)
{
	t_vec3f	vf;

	vf.x = (float)vd.x;
	vf.y = (float)vd.y;
	vf.z = (float)vd.z;
	vf.w = (float)0.0;
	return (vf);
}
