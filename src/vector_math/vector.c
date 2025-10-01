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
#include <xmmintrin.h>

FORCEINLINE
extern inline t_vec3f	vec3f_min(t_vec3f a, t_vec3f b)
{
	return ((t_vec3f){
		fminf(a.x, b.x),
		fminf(a.y, b.y),
		fminf(a.z, b.z)
	});
}

FORCEINLINE
extern inline t_vec3f	vec3f_max(t_vec3f a, t_vec3f b)
{
	return ((t_vec3f){
		fmaxf(a.x, b.x),
		fmaxf(a.y, b.y),
		fmaxf(a.z, b.z)
	});
}

FORCEINLINE
extern inline t_vec3f	vec3f_sub(t_vec3f a, t_vec3f b)
{
	return ((t_vec3f){
		a.x - b.x,
		a.y - b.y,
		a.z - b.z,
	});
}

FORCEINLINE
extern inline t_vec3f	vec3f_add(t_vec3f a, t_vec3f b)
{
	return ((t_vec3f){
		a.x + b.x,
		a.y + b.y,
		a.z + b.z,
	});
}

FORCEINLINE
extern inline t_vec3f	vec3f_scale(t_vec3f a, float s)
{
	return ((t_vec3f){
		a.x * s,
		a.y * s,
		a.z * s,
	});
}

FORCEINLINE
extern inline float	vec3f_dot(t_vec3f a, t_vec3f b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
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

#if defined(__SSE__)

// one refinement y1 = y0 * (1.5 - 0.5 * x * y0^2)
// add another one if subtle floating-point errors we dont like
FORCEINLINE
extern inline float	ft_rsqrtf(float x)
{
	__m128	half;
	__m128	three;
	__m128	y0;
	__m128	y1;
	__m128	x000;

	half = _mm_set_ss(0.5f);
	three = _mm_set_ss(3.0f);
	x000 = _mm_set_ss(x);
	y0 = _mm_rsqrt_ss(x000);
	y1 = _mm_mul_ss(
		_mm_mul_ss(y0, half),
		_mm_sub_ss(three, _mm_mul_ss(_mm_mul_ss(x000, y0), y0))
	);
	return (_mm_cvtss_f32(y1));
}

#else

FORCEINLINE
extern inline float	ft_rsqrtf(float x)
{
	return (1.f / sqrtf(x));
}

#endif

FORCEINLINE
extern inline t_vec3f	vec3f_normalize(t_vec3f a)
{
	return (vec3f_scale(a, ft_rsqrtf(vec3f_dot(a, a))));
}
