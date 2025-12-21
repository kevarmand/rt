/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_math.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 15:49:18 by norivier          #+#    #+#             */
/*   Updated: 2025/12/21 06:47:36 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_MATH_H
# define RT_MATH_H 1

# ifndef EPSILON
#  define EPSILON 1.0e-6f
# endif
# define ZEROF 1.0e-8f

float	ft_rsqrtf(float x);
float	ft_rcpf(float x);

#endif // !RT_MATH_H
