/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: norivier <norivier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 16:47:11 by norivier          #+#    #+#             */
/*   Updated: 2025/09/11 16:51:25 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_H
# define VECTOR_H 1

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

#endif // !VECTOR_H
