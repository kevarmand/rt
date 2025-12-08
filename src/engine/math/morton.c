/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   morton.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: norivier <norivier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 23:46:26 by norivier          #+#    #+#             */
/*   Updated: 2025/11/11 00:00:18 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "types.h"
#include "vector.h"
#include <math.h>
#include <stdint.h>

FORCEINLINE
extern inline uint32_t	expandbits(uint32_t v)
{
	v = (v * 0x00010001u) & 0xFF0000FFu;
	v = (v * 0x00000101u) & 0x0F00F00Fu;
	v = (v * 0x00000011u) & 0xC30C30C3u;
	v = (v * 0x00000005u) & 0x49249249u;
	return (v);
}

FORCEINLINE
extern inline uint32_t	morton3D(t_vec3f v)
{
	v.x = fminf(fmaxf(v.x * 1024.0f, 0), 1023.0f);
	v.y = fminf(fmaxf(v.y * 1024.0f, 0), 1023.0f);
	v.z = fminf(fmaxf(v.z * 1024.0f, 0), 1023.0f);
	return (expandbits((uint32_t)v.x) << 2
		| expandbits((uint32_t)v.y) << 1
		| expandbits((uint32_t)v.z));
}
