/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_ray.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 16:14:40 by norivier          #+#    #+#             */
/*   Updated: 2025/12/18 15:06:36 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"
#include "engine.h"
#include "attributes.h"
#include <stdint.h>

FORCEINLINE
t_ray	ray_finalize(t_vec3f origin, t_vec3f direction)
{
	t_ray	out;

	out.origin = origin;
	out.dir = direction;
	out.invdir = 1.0f / direction;
	out.orig_div = origin * out.invdir;
	out.sign[0] = (out.invdir.x < 0);
	out.sign[1] = (out.invdir.y < 0);
	out.sign[2] = (out.invdir.z < 0);
	return (out);
}
