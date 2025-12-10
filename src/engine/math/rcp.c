/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rcp.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: norivier <norivier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 15:52:39 by norivier          #+#    #+#             */
/*   Updated: 2025/12/10 04:27:12 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "attributes.h"
#include <immintrin.h>

#if defined(__SSE__)

FORCEINLINE
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

FORCEINLINE
extern inline float	ft_rcpf(float x)
{
	return (1.0f / x);
}

#endif
