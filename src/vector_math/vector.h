/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: norivier <norivier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 16:47:11 by norivier          #+#    #+#             */
/*   Updated: 2025/09/14 20:45:03 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_H
# define VECTOR_H 1

# if defined(_MSC_VER)
#  define FORCEINLINE __forceinline
# elif defined(__GNUC__) || defined(__clang__)
#  define FORCEINLINE __attribute__((always_inline))
# endif

typedef struct s_vec3f		t_vec3f;
typedef struct s_vec3fsse	t_vec3fsse;
typedef struct s_vec3favx	t_vec3avx;

# include <immintrin.h>

struct s_vec3f
{
	float	x;
	float	y;
	float	z;
};

struct s_vec3fsse
{
	__m128	x;
	__m128	y;
	__m128	z;
};

struct s_vec3favx
{
	__m256	x;
	__m256	y;
	__m256	z;
};

t_vec3f	vec3f_min(t_vec3f a, t_vec3f b);
t_vec3f	vec3f_max(t_vec3f a, t_vec3f b);
t_vec3f	vec3f_cross(t_vec3f a, t_vec3f b);
float	vec3f_dot(t_vec3f a, t_vec3f b);
t_vec3f	vec3f_scale(t_vec3f a, float s);
t_vec3f	vec3f_add(t_vec3f a, t_vec3f b);
t_vec3f	vec3f_sub(t_vec3f a, t_vec3f b);

#endif // !VECTOR_H
