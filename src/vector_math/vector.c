/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: norivier <norivier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 20:38:58 by norivier          #+#    #+#             */
/*   Updated: 2025/09/14 20:44:21 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"
#include <math.h>

__attribute__((always_inline))
extern inline t_vec3f	vec3f_min(t_vec3f a, t_vec3f b)
{
	return ((t_vec3f){
		fminf(a.x, b.x),
		fminf(a.y, b.y),
		fminf(a.z, b.z)
	});
}

__attribute__((always_inline))
extern inline t_vec3f	vec3f_max(t_vec3f a, t_vec3f b)
{
	return ((t_vec3f){
		fmaxf(a.x, b.x),
		fmaxf(a.y, b.y),
		fmaxf(a.z, b.z)
	});
}

__attribute__((always_inline))
extern inline t_vec3f	vec3f_sub(t_vec3f a, t_vec3f b)
{
	return ((t_vec3f){
		a.x - b.x,
		a.y - b.y,
		a.z - b.z,
	});
}

__attribute__((always_inline))
extern inline t_vec3f	vec3f_add(t_vec3f a, t_vec3f b)
{
	return ((t_vec3f){
		a.x + b.x,
		a.y + b.y,
		a.z + b.z,
	});
}

__attribute__((always_inline))
extern inline t_vec3f	vec3f_scale(t_vec3f a, float s)
{
	return ((t_vec3f){
		a.x * s,
		a.y * s,
		a.z * s,
	});
}

__attribute__((always_inline))
extern inline float	vec3f_dot(t_vec3f a, t_vec3f b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

__attribute__((always_inline))
extern inline t_vec3f	vec3f_cross(t_vec3f a, t_vec3f b)
{
	return ((t_vec3f){
		a.y * b.z - a.z * b.y,
		a.z * b.x - b.z * a.x,
		a.x * b.y - a.y * b.x,
	});
}
