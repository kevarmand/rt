/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 19:21:07 by norivier          #+#    #+#             */
/*   Updated: 2025/12/11 22:10:04 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "types.h"
#include "vector.h"

// FORCEINLINE
// extern inline	t_mat3x3f	mat3x3_inv(t_mat3x3f m)
// {
// 	t_mat3x3f	cofactor;
// 	float		invdet;

// 	cofactor[0][0] = m[1][1] * m[2][2] - m[1][2] * m[2][1];
// 	cofactor[0][1] = -m[1][0] * m[2][2] + m[1][2] * m[2][0];
// 	cofactor[0][2] = m[1][0] * m[2][1] - m[1][1] * m[2][0];
// 	cofactor[1][0] = -m[0][1] * m[2][2] + m[0][2] * m[2][1];
// 	cofactor[1][1] = m[0][0] * m[2][2] - m[0][2] * m[2][0];
// 	cofactor[1][2] = -m[0][0] * m[2][1] + m[0][1] * m[2][0];
// 	cofactor[2][0] = m[0][1] * m[1][2] - m[0][2] * m[1][1];
// 	cofactor[2][1] = -m[0][0] * m[1][2] + m[0][2] * m[1][0];
// 	cofactor[2][2] = m[0][0] * m[1][1] - m[0][1] * m[1][0];
// 	invdet = 1.0f / (m[0][0] * cof[0][0]
// 		+ m[0][1] * cof[1][0]
// 		+ m[0][2] * cof[2][0]);
// 	return (__builtin_matrix_transpose(cofactor) * invdet);
// }
