/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   equation_solvers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: norivier <norivier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 02:20:09 by norivier          #+#    #+#             */
/*   Updated: 2025/12/10 04:29:45 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <complex.h>
#include <math.h>
#include "types.h"
#include "rt_math.h"
#include "bvh.h"
#define ZEROF 1.0e-8f

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

// Cardano with Viete fallback cubic solver. i don't think i can
// optimize it further
FORCEINLINE
extern inline int	solve_cubic(t_equ arg, float roots[])
{
	float	inv_a;
	float	s;
	float	q_half;
	float	p_third;
	float	cardano_disc;
	float	disc_sqrt;
	float	r;
	float	r2;
	float	phi;

	if (is_zero(arg.a))
		return (solve_quad((t_equ){arg.b, arg.c, arg.d, 0, 0}, roots));
	inv_a = ft_rcpf(arg.a);
	arg.b *= inv_a;
	arg.c *= inv_a;
	arg.d *= inv_a;
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
	else
	{
		r = sqrtf(fmaxf(-p_third, 0.0f));
		phi = acosf(fmaxf(fminf(-q_half / (r * r * r), 1.0f), -1.0f));
		r2 = 2.0f * r;
		roots[0] = r2 * cosf(phi * RCP_3) - s;
		roots[1] = r2 * cosf((phi + 2.0f * (float)M_PI) * RCP_3) - s;
		roots[2] = r2 * cosf((phi + 4.0f * (float)M_PI) * RCP_3) - s;
		sortf3(&roots[0], &roots[1], &roots[2]);
		return (3);
	}
}
