/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rcp.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: norivier <norivier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 15:52:39 by norivier          #+#    #+#             */
/*   Updated: 2025/12/21 06:41:25 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <immintrin.h>

#if defined(__SSE__)

__attribute__((always_inline))
extern inline float	ft_rcpf(float x)
{
	__m128			res;
	__m128			muls;
	__m128			x000;

	x000 = _mm_set_ss(x);
	res = _mm_rcp_ss(x000);
	muls = _mm_mul_ss(x000, _mm_mul_ss(res, res));
	res = _mm_sub_ss(_mm_add_ss(res, res), muls);
	return (_mm_cvtss_f32(res));
}

#else

__attribute__((always_inline))
extern inline float	ft_rcpf(float x)
{
	return (1.0f / x);
}

#endif
