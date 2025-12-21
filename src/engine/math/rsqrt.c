/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rsqrt.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: norivier <norivier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 15:53:19 by norivier          #+#    #+#             */
/*   Updated: 2025/12/21 06:40:01 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <immintrin.h>
#include <math.h>

#if defined(__SSE__)

__attribute__((always_inline))
extern inline float	ft_rsqrtf(float x)
{
	const __m128	three = _mm_set_ss(3.0f);
	const __m128	half = _mm_set_ss(0.5f);
	__m128			res;
	__m128			muls;
	__m128			x000;

	x000 = _mm_set_ss(x);
	res = _mm_rsqrt_ss(x000);
	muls = _mm_mul_ss(_mm_mul_ss(x000, res), res);
	res = _mm_mul_ss(_mm_mul_ss(half, res), _mm_sub_ss(three, muls));
	return (_mm_cvtss_f32(res));
}

#else

__attribute__((always_inline))
extern inline float	ft_rsqrtf(float x)
{
	return (1.0f / sqrtf(x));
}

#endif
