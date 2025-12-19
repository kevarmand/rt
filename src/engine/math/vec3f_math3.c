/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3f_math3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: norivier <norivier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 17:56:57 by norivier          #+#    #+#             */
/*   Updated: 2025/12/18 18:05:13 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "vector.h"
#include "attributes.h"

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
extern inline float	vec3f_length(t_vec3f a)
{
	return (sqrtf(vec3f_dot(a, a)));
}

FORCEINLINE
extern inline float	vec3f_norme(t_vec3f a)
{
	return (sqrtf(vec3f_dot(a, a)));
}
