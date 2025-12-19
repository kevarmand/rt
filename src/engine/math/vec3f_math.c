/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3f_math.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: norivier <norivier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 20:38:58 by norivier          #+#    #+#             */
/*   Updated: 2025/12/18 18:05:36 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"
#include "attributes.h"
#include "rt_math.h"

FORCEINLINE
extern inline t_vec3f	vec3f_normalize(t_vec3f a)
{
	return (vec3f_scale(a, ft_rsqrtf(vec3f_dot(a, a))));
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
