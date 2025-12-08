/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rsqrt.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
<<<<<<< HEAD:src/engine/math/rsqrt.c
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 21:24:45 by norivier          #+#    #+#             */
/*   Updated: 2025/11/10 14:37:39 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "attributes.h"
=======
/*   By: norivier <norivier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 21:24:45 by norivier          #+#    #+#             */
/*   Updated: 2025/10/02 10:16:52 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "types.h"
>>>>>>> readme:src/vector_math/rsqrt.c
#include <immintrin.h>
#include <math.h>

#if defined(__SSE__)

FORCEINLINE
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

FORCEINLINE
extern inline float	ft_rsqrtf(float x)
{
	return (1.0f / sqrtf(x));
}

<<<<<<< HEAD:src/engine/math/rsqrt.c
#endif
=======
#endif
>>>>>>> readme:src/vector_math/rsqrt.c
