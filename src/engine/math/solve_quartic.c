/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solve_quartic.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: norivier <norivier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 06:07:40 by norivier          #+#    #+#             */
/*   Updated: 2025/12/21 06:11:37 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <complex.h>
#include <math.h>
#include "libft.h"
#include "rt_math.h"
#include "bvh.h"

__attribute__((always_inline))
static inline float	dcubic(float p, float q)
{
	float	h;
	float	r;
	float	u;
	float	k;

	h = q * q + 4.0 * p * p * p;
	r = sqrtf(fabsf(h));
	if (h >= 0.0)
	{
		u = cbrtf(-0.5 * (q + copysignf(r, q)));
		return (u - p / u);
	}
	else
	{
		k = 2.0 * sqrtf(fabsf(p));
		return (k * cosf(atan2f(r, -q) * (1.0 / 3.0)));
	}
}

__attribute__((always_inline))
static inline float	qcubic1(float b, float c, float d)
{
	float	p;
	float	q;
	float	x;

	b *= (1.0 / 3.0);
	c *= (1.0 / 3.0);
	p = c - b * b;
	q = 2.0 * b * b * b - 3.0 * b * c + d;
	x = dcubic(p, q);
	x -= b;
	return (x);
}

__attribute__((always_inline))
static inline float	qcubic(float b, float c, float d)
{
	float	x;
	float	fx;
	float	f1x;
	int		i;

	x = qcubic1(b, c, d);
	i = 0;
	while (i < 1)
	{
		fx = d + x * (c + x * (b + x));
		f1x = c + x * (2.0 * b + 3.0 * x);
		if (fabsf(f1x) > ZEROF)
			x -= fx / f1x;
		i += 1;
	}
	return (x);
}

typedef struct s_lanczos_ctx
{
	float	alpha;
	float	big_a;
	float	big_b;
	float	a;
	float	b;
	float	beta;
	float	psi;
}	t_lanczos_ctx;

// Lanczos
__attribute__((always_inline))
static inline int	solve_quartic0(t_equ arg, float roots[])
{
	t_lanczos_ctx	lz;
	int				n0;
	int				n1;

	lz.alpha = 0.5 * arg.b;
	lz.big_a = arg.c - lz.alpha * lz.alpha;
	lz.big_b = arg.d - lz.alpha * lz.big_a;
	lz.psi = qcubic(2.0 * lz.big_a - lz.alpha * lz.alpha, lz.big_a * lz.big_a
			+ 2.0 * lz.big_b * lz.alpha - 4.0 * arg.e, -lz.big_b * lz.big_b);
	lz.psi = fmaxf(0.0, lz.psi);
	lz.a = sqrtf(lz.psi);
	lz.beta = 0.5 * (lz.big_a + lz.psi);
	if (lz.psi <= 0.0)
		lz.b = sqrtf(fmaxf(lz.beta * lz.beta - arg.e, 0.0));
	else
		lz.b = 0.5 * lz.a * (lz.alpha - lz.big_b / lz.psi);
	n0 = solve_quad((t_equ){1.0, lz.alpha + lz.a, lz.beta + lz.b, 0, 0}, roots);
	n1 = solve_quad((t_equ){1.0, lz.alpha - lz.a, lz.beta - lz.b, 0, 0},
			roots + 2);
	if (n0 == 0)
	{
		roots[0] = roots[2];
		roots[1] = roots[3];
	}
	return (n0 + n1);
}

__attribute__((always_inline, flatten))
extern inline int	solve_quarticf(t_equ arg, float roots[])
{
	int		nroots;
	int		flip;
	t_equ	eq;
	int		i;

	set_equ(&eq, arg, &flip);
	ft_bzero(roots, sizeof(float) * 4);
	nroots = solve_quartic0(eq, roots);
	if (flip)
	{
		i = 0;
		while (i < 4)
		{
			roots[i] = 1.0 / roots[i];
			i += 1;
		}
	}
	return (nroots);
}
