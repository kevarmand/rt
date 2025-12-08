/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   order.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: norivier <norivier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 02:13:04 by norivier          #+#    #+#             */
/*   Updated: 2025/10/31 02:16:20 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "types.h"

FORCEINLINE
extern inline void	swap_int(int *a, int *b)
{
	int	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

FORCEINLINE
extern inline void	swap_float(float *a, float *b)
{
	float	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

FORCEINLINE
extern inline void	sortf2(float *a, float *b)
{
	if (*a > *b)
		swap_float(a, b);
}

FORCEINLINE
extern inline void	sortf3(float *a, float *b, float *c)
{
	if (*a > *b)
		swap_float(a, b);
	if (*b > *c)
		swap_float(b, c);
	if (*a > *b)
		swap_float(a, b);
}

FORCEINLINE
extern inline void	sortf4(float *a, float *b, float *c, float *d)
{
	if (*a > *b)
		swap_float(a, b);
	if (*c > *d)
		swap_float(c, d);
	if (*a > *c)
		swap_float(a, c);
	if (*b > *d)
		swap_float(b, d);
	if (*b > *c)
		swap_float(b, c);
}
