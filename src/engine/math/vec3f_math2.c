/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3f_math2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 17:53:39 by norivier          #+#    #+#             */
/*   Updated: 2025/12/19 18:15:15 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"
#include "attributes.h"

FORCEINLINE
extern inline float	vec3f_dot(t_vec3f a, t_vec3f b)
{
	t_vec3f	tmp;

	tmp = a * b;
	return (tmp.x + tmp.y + tmp.z);
}

FORCEINLINE
extern inline t_vec3f	vec3f_cross(t_vec3f a, t_vec3f b)
{
	return ((t_vec3f){
		a.y * b.z - a.z * b.y,
		a.z * b.x - b.z * a.x,
		a.x * b.y - a.y * b.x,
	});
}

FORCEINLINE
extern inline t_vec3f	vec3f_mul(t_vec3f a, t_vec3f b)
{
	return ((t_vec3f){
		a.x * b.x,
		a.y * b.y,
		a.z * b.z
	});
}

FORCEINLINE
extern inline t_vec3f	proj_on_plane(t_vec3f v, t_vec3f n)
{
	return (vec3f_sub(v, vec3f_scale(n, vec3f_dot(v, n))));
}

FORCEINLINE
extern inline void	vec3f_load3(t_vec3f *dst, const float src[3])
{
	(*dst)[0] = src[0];
	(*dst)[1] = src[1];
	(*dst)[2] = src[2];
	(*dst)[3] = 0.0f;
}
