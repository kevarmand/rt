/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 20:38:58 by norivier          #+#    #+#             */
/*   Updated: 2025/12/18 21:27:27 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "vector.h"
#include "attributes.h"
#include "rt_math.h"

FORCEINLINE
extern inline t_vec3f	vec3f_min(t_vec3f a, t_vec3f b)
{
	return ((t_vec3f){
		fminf(a.x, b.x),
		fminf(a.y, b.y),
		fminf(a.z, b.z),
		0.0f
	});
}

FORCEINLINE
extern inline t_vec3f	vec3f_max(t_vec3f a, t_vec3f b)
{
	return ((t_vec3f){
		fmaxf(a.x, b.x),
		fmaxf(a.y, b.y),
		fmaxf(a.z, b.z),
		0.0f
	});
}

FORCEINLINE
extern inline t_vec3f	vec3f_sub(t_vec3f a, t_vec3f b)
{
	return (a - b);
}

FORCEINLINE
extern inline t_vec3f	vec3f_add(t_vec3f a, t_vec3f b)
{
	return (a + b);
}

FORCEINLINE
extern inline t_vec3f	vec3f_scale(t_vec3f a, float s)
{
	return (a * (t_vec3f){s, s, s, 0});
}

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
extern inline float	vec3f_norme(t_vec3f a)
{
	return (sqrtf(vec3f_dot(a, a)));
}

FORCEINLINE
extern inline t_vec3f	vec3f_normalize(t_vec3f a)
{
	return (vec3f_scale(a, ft_rsqrtf(vec3f_dot(a, a))));
}

FORCEINLINE
extern inline void	vec3f_load3(t_vec3f *dst, const float src[3])
{
	(*dst)[0] = src[0];
	(*dst)[1] = src[1];
	(*dst)[2] = src[2];
	(*dst)[3] = 0.0f;
}

FORCEINLINE
extern inline float	vec3f_length(t_vec3f a)
{
	return (sqrtf(vec3f_dot(a, a)));
}


FORCEINLINE
extern inline t_vec3f vec3f_mul(t_vec3f a, t_vec3f b)
{
	return ((t_vec3f){
		a.x * b.x,
		a.y * b.y,
		a.z * b.z
	});
}


FORCEINLINE
extern inline t_vec3f proj_on_plane(t_vec3f v, t_vec3f n)
{
	return (vec3f_sub(v, vec3f_scale(n, vec3f_dot(v, n))));
}

FORCEINLINE
extern inline t_vec3f	vec3f_lerp(t_vec3f a, t_vec3f b, float t)
{
	return (vec3f_add(vec3f_scale(a, 1.0f - t), vec3f_scale(b, t)));
}
