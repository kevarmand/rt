// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   vector_sse.c                                       :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: norivier <norivier@student.42.fr>          +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2025/09/11 16:51:53 by norivier          #+#    #+#             */
// /*   Updated: 2025/09/11 18:42:55 by norivier         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "vector.h"
// #include <xmmintrin.h>

// __attribute__((always_inline))
// inline	void	vec3fsse_set(t_vec3fsse *v, const float *p)
// {
// 	_mm_load_ps(p);
// }

// __attribute__((always_inline))
// inline	__m128	vec3fsse_dot(t_vec3fsse *a, t_vec3fsse *b)
// {
// 	return (
// 		_mm_add_ps(
// 			_mm_add_ps(
// 				_mm_mul_ps(a->x, b->x),
// 				_mm_mul_ps(a->y, b->y)
// 			),
// 			_mm_mul_ps(a->z, b->z))
// 	);
// }

// __attribute__((always_inline))
// inline	t_vec3fsse	vec3fsse_cross(t_vec3fsse *a, t_vec3fsse *b)
// {
// 	return ((t_vec3fsse){
// 		_mm_sub_ps(
// 			_mm_mul_ps(a->y, b->z),
// 			_mm_mul_ps(a->z, b->y)
// 		),
// 		_mm_sub_ps(
// 			_mm_mul_ps(a->z, b->x),
// 			_mm_mul_ps(a->x, b->z)
// 		),
// 		_mm_sub_ps(
// 			_mm_mul_ps(a->x, b->y),
// 			_mm_mul_ps(a->y, b->x)
// 		),
// 	});
// }

// __attribute__((always_inline))
// inline	t_vec3fsse	vec3fsse_mul(t_vec3fsse *a, __m128 *p)
// {
// }
// // https://stackoverflow.com/questions/35885170/handling-zeroes-in-mm256-rsqrt-ps
// // https://stackoverflow.com/questions/1528727/why-is-sse-scalar-sqrtx-slower-than-rsqrtx-x/1528751#1528751
// // https://stackoverflow.com/questions/31555260/fast-vectorized-rsqrt-and-reciprocal-with-sse-avx-depending-on-precision
// __attribute__((always_inline))
// inline	t_vec3fsse	vec3fsse_normalize(t_vec3fsse *a)
// {

// }
