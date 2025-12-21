/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   order.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 02:13:04 by norivier          #+#    #+#             */
/*   Updated: 2025/12/21 06:55:07 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

__attribute__((always_inline))
extern inline void	swap_int(int *a, int *b)
{
	int	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

__attribute__((always_inline))
extern inline void	swap_float(float *a, float *b)
{
	float	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

__attribute__((always_inline))
extern inline void	sortf2(float *a, float *b)
{
	if (*a > *b)
		swap_float(a, b);
}

__attribute__((always_inline))
extern inline void	sortf3(float *a, float *b, float *c)
{
	if (*a > *b)
		swap_float(a, b);
	if (*b > *c)
		swap_float(b, c);
	if (*a > *b)
		swap_float(a, b);
}
