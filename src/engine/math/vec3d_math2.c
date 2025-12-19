/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3d_math2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: norivier <norivier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 18:02:02 by norivier          #+#    #+#             */
/*   Updated: 2025/12/18 18:03:55 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "vector.h"
#include "attributes.h"

FORCEINLINE
extern inline double	vec3d_length(t_vec3d a)
{
	return (sqrt(vec3d_dot(a, a)));
}

FORCEINLINE
extern inline t_vec3d	vec3d_normalize(t_vec3d a)
{
	return (vec3d_scale(a, 1.0 / vec3d_length(a)));
}
