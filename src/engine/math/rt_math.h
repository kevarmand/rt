
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_math.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: norivier <norivier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 21:29:13 by norivier          #+#    #+#             */
/*   Updated: 2025/10/02 10:24:57 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_MATH_H
# define RT_MATH_H 1

#define EPSILON 0x1.0c6f7ap-20f
#define RCP_2 0x1p-1f
#define RCP_3 0x1.555556p-2f
#define RCP_4 0x1p-2f
#define RCP_8 0x1p-3f
#define RCP_9 0x1.c71c72p-4f
#define RCP_16 0x1p-4f
#define RCP_256 0x1p-8f
#define THREE_OVER_8 0x1.8p-2f
#define THREE_OVER_256 0x1.8p-2f

float	ft_rsqrtf(float x);
float	ft_rcpf(float x);

#endif // !RT_MATH_H
