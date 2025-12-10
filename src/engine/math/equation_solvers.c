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

#include <math.h>
#include "types.h"
#include "rt_math.h"
#include "bvh.h"

// Stable version
FORCEINLINE
extern inline int	solve_quad(t_equ arg, float roots[])
{
	float	disc;
	float	disc_sqrt;
	float	q;

	if (fabsf(arg.a) < (float)EPSILON)
	{
		if (fabsf(arg.b) < (float)EPSILON)
			return (0);
		roots[0] = -arg.c / arg.b;
		return (1);
	}
	disc = arg.b * arg.b - 4.0f * arg.a * arg.c;
	if (disc < 0.0f)
		return (0);
	disc_sqrt = sqrtf(disc);
	q = -0.5f * (arg.b + copysignf(disc_sqrt, arg.b));
	roots[0] = q / arg.a;
	roots[1] = arg.c / q;
	sortf2(&roots[0], &roots[1]);
	if (disc > (float)EPSILON)
		return (2);
	return (1);
}

// Cardano with Viete fallback cubic solver. i don't think i can
// optimize it further
FORCEINLINE
extern inline int	solve_cubic(t_equ arg, float roots[])
{
	float	inv_a;
	float	b_over_3;
	float	b_squared_over_3;
	float	q_half;
	float	p_third;
	float	p;
	float	q;
	float	cardano_disc;
	float	disc_sqrt;
	float	t1;
	float	t2;
	float	r;
	float	r2;
	float	phi;

	if (fabsf(arg.a) < EPSILON)
		return (solve_quad((t_equ){arg.b, arg.c, arg.d, 0, 0}, roots));
	inv_a = ft_rcpf(arg.a);
	arg.b *= inv_a;
	arg.c *= inv_a;
	arg.d *= inv_a;
	b_over_3 = arg.b * RCP_3;
	b_squared_over_3 = arg.b * b_over_3;
	p = arg.c - b_squared_over_3;
	q = (2.0f * b_squared_over_3 * arg.b * RCP_9)
		- (arg.c * b_over_3) + arg.d;
	q_half = q * 0.5f;
	p_third = p * RCP_3;
	cardano_disc = q_half * q_half + p_third * p_third * p_third;
	if (cardano_disc > 0.0f)
	{
		disc_sqrt = sqrtf(cardano_disc);
		t1 = -q_half + disc_sqrt;
		t2 = -q_half - disc_sqrt;
		roots[0] = copysignf(cbrtf(fabsf(t1)), t1)
			+ copysignf(cbrtf(fabsf(t2)), t2) - b_over_3;
		return (1);
	}
	else
	{
		r = sqrtf(-p_third);
		phi = acosf(fmaxf(fminf(-q_half / (r * r * r), 1.0f), -1.0f));
		r2 = 2.0f * r;
		roots[0] = r2 * cosf(phi * RCP_3) - b_over_3;
		roots[1] = r2 * cosf((phi + 2.0f * (float)M_PI) * RCP_3) - b_over_3;
		roots[2] = r2 * cosf((phi + 4.0f * (float)M_PI) * RCP_3) - b_over_3;
		sortf3(&roots[0], &roots[1], &roots[2]);
		return (3);
	}
}

// ferrari it appears
FORCEINLINE
extern inline int	solve_quartic(t_equ arg, float roots[])
{
	float	inv_a;
	float	b2;
	float	p;
	float	q;
	float	r;
	float	z;
	float	u;
	float	disc;
	float	v;
	int		n;
	int		i;
	float	substitute;

	if (fabsf(arg.a) < EPSILON)
		return (solve_cubic((t_equ){arg.b, arg.c, arg.d, arg.e, 0}, roots));
	inv_a = ft_rcpf(arg.a);
	arg.b *= inv_a;
	arg.c *= inv_a;
	arg.d *= inv_a;
	arg.e *= inv_a;
	b2 = arg.b * arg.b;
	p = arg.c - THREE_OVER_8 * b2;
	q = RCP_8 * b2 * arg.b - (RCP_2 * arg.b * arg.c) + arg.d;
	r = -THREE_OVER_256 * b2 * b2 + RCP_16 * b2 * arg.c
		- RCP_4 * arg.b * arg.d + arg.e;
	// z3 - z2p/2 - rz + (rp/2 - q2/8) one root;
	(void)solve_cubic((t_equ){1.0f, -RCP_2 * p, -r,
		-RCP_2 * r * p - RCP_8 * q * q, 0}, roots);
	z = roots[0];
	u = sqrtf(fmaxf(2.0f * z - p, 0.0f));
	disc = -4.0f * z * z + 4.0f * r + (q * q) / (u * u);
	if (fabsf(u) < EPSILON || fabsf(disc) < EPSILON)
		v = 0.0f;
	else
		v = -q / (2.0f * u);
	n = solve_quad((t_equ){1.0f, u, z - v, 0, 0}, roots);
	n += solve_quad((t_equ){1.0f, -u, z + v, 0, 0}, roots + n);
	substitute = arg.b * RCP_4;
	i = 0;
	while (i < n)
	{
		roots[i] -= substitute; // x = y - b/4
		i += 1;
	}
	if (n == 2)
		sortf2(&roots[0], &roots[1]);
	else if (n == 3)
		sortf3(&roots[0], &roots[1], &roots[2]);
	else if (n == 4)
		sortf4(&roots[0], &roots[1], &roots[2], &roots[3]);
	return (n);
}
