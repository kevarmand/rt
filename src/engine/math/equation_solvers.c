/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   equation_solvers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 02:20:09 by norivier          #+#    #+#             */
/*   Updated: 2025/12/19 18:14:39 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <complex.h>
#include <math.h>
#include "attributes.h"
#include "rt_math.h"
#include "bvh.h"

FORCEINLINE
static inline int	is_zero(float x)
{
	return (fabsf(x) < ZEROF);
}

// Stable version
FORCEINLINE
extern inline int	solve_quad(t_equ arg, float roots[])
{
	float	disc;
	float	disc_sqrt;
	float	q;

	if (is_zero(arg.a))
	{
		if (is_zero(arg.b))
			return (0);
		roots[0] = -arg.c / arg.b;
		return (1);
	}
	disc = arg.b * arg.b - 4.0f * arg.a * arg.c;
	if (disc < -ZEROF)
		return (0);
	disc_sqrt = sqrtf(fmaxf(disc, 0.0f));
	q = -RCP_2 * (arg.b + copysignf(disc_sqrt, arg.b));
	roots[0] = q / arg.a;
	roots[1] = arg.c / q;
	sortf2(&roots[0], &roots[1]);
	if (disc > ZEROF)
		return (2);
	return (1);
}

FORCEINLINE
static inline int	geometric_fallback(float q_half, float p_third,
		float shift, float roots[])
{
	float	r;
	float	r2;
	float	phi;

	r = sqrtf(fmaxf(-p_third, 0.0f));
	phi = acosf(fmaxf(fminf(-q_half / (r * r * r), 1.0f), -1.0f));
	r2 = 2.0f * r;
	roots[0] = r2 * cosf(phi * RCP_3) - shift;
	roots[1] = r2 * cosf((phi + 2.0f * (float)M_PI) * RCP_3) - shift;
	roots[2] = r2 * cosf((phi + 4.0f * (float)M_PI) * RCP_3) - shift;
	sortf3(&roots[0], &roots[1], &roots[2]);
	return (3);
}

// Cardano with Viete fallback cubic solver. i don't think i can
// optimize it further
FORCEINLINE
extern inline int	solve_cubic(t_equ arg, float roots[])
{
	float	s;
	float	q_half;
	float	p_third;
	float	cardano_disc;
	float	disc_sqrt;

	if (is_zero(arg.a))
		return (solve_quad((t_equ){arg.b, arg.c, arg.d, 0, 0}, roots));
	arg.b /= arg.a;
	arg.c /= arg.a;
	arg.d /= arg.a;
	s = arg.b * RCP_3;
	p_third = arg.c * RCP_3 - s * s;
	q_half = s * s * s + (arg.d - s * arg.c) * RCP_2;
	cardano_disc = q_half * q_half + p_third * p_third * p_third;
	if (cardano_disc > ZEROF)
	{
		disc_sqrt = sqrtf(cardano_disc);
		roots[0] = cbrtf(disc_sqrt - q_half) + cbrtf(-disc_sqrt - q_half) - s;
		return (1);
	}
	return (geometric_fallback(q_half, p_third, s, roots));
}

float	dcubic(float p, float q)
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

float	qcubic1(float b, float c, float d)
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

float	qcubic(float b, float c, float d)
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

// Lanczos
FORCEINLINE
extern inline int	solve_quartic0(t_equ arg, float roots[])
{
	float	alpha;
	float	A;
	float	B;
	float	a;
	float	b;
	float	beta;
	float	psi;
	int		n0;
	int		n1;

	alpha = 0.5 * arg.b;
	A = arg.c - alpha * alpha;
	B = arg.d - alpha * A;
	psi = qcubic(2.0 * A - alpha * alpha, A * A + 2.0 * B * alpha - 4.0 * arg.e, -B * B);
	psi = fmaxf(0.0, psi);
	a = sqrtf(psi);
	beta = 0.5 * (A + psi);
	if (psi <= 0.0)
		b = sqrtf(fmaxf(beta * beta - arg.e, 0.0));
	else
		b = 0.5 * a * (alpha - B / psi);
	n0 = solve_quad((t_equ){1.0, alpha + a, beta + b, 0, 0}, roots);
	n1 = solve_quad((t_equ){1.0, alpha - a, beta - b, 0, 0}, roots + 2);
	if (n0 == 0)
	{
		roots[0] = roots[2];
		roots[1] = roots[3];
	}
	return (n0 + n1);
}

FORCEINLINE
extern inline int	solve_quarticf(t_equ arg, float roots[])
{
	int	nroots;
	int	flip;
	float	b;
	float	c;
	float	d;
	float	e;

	flip = fabsf(arg.b / arg.a) >= fabsf(arg.d / arg.e);
	if (!flip)
	{
		b = arg.b / arg.a;
		c = arg.c / arg.a;
		d = arg.d / arg.a;
		e = arg.e / arg.a;
	}
	else
	{
		b = arg.d / arg.e;
		c = arg.c / arg.e;
		d = arg.b / arg.e;
		e = arg.a / arg.e;
	}
	roots[0] = 0.0;
	roots[1] = 0.0;
	roots[2] = 0.0;
	roots[3] = 0.0;
	nroots = solve_quartic0((t_equ){1.0, b, c, d, e}, roots);
	if (flip)
	{
		int	i;

		i = 0;
		while (i < 4)
		{
			roots[i] = 1.0 / roots[i];
			i += 1;
		}
	}
	return (nroots);
}
