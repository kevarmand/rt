/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_math.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 15:49:18 by norivier          #+#    #+#             */
/*   Updated: 2025/12/19 18:12:42 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_MATH_H
# define RT_MATH_H 1

# ifndef EPSILON
#  define EPSILON 0x1.0c6f7ap-20f
# endif
# define RCP_2 0x1p-1f
# define RCP_3 0x1.555556p-2f
# define RCP_4 0x1p-2f
# define RCP_8 0x1p-3f
# define RCP_9 0x1.c71c72p-4f
# define RCP_12 0x1.555556p-4f
# define RCP_16 0x1p-4f
# define RCP_27 0x1.0f0f0fp-5f
# define RCP_54 0x1.2f5c28p-6f
# define RCP_108 0x1.2e8ba3p-7f
# define RCP_256 0x1p-8f
# define THREE_OVER_8 0x1.8p-2f
# define THREE_OVER_256 0x1.8p-2f
# define FIVE_OVER_6 0x1.aaaaaap-1f
# define ZEROF 1.0e-8f
# define ZEROD 1.0e-10

float	ft_rsqrtf(float x);
float	ft_rcpf(float x);

#endif // !RT_MATH_H
