/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cequation_solvers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: norivier <norivier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 19:22:32 by norivier          #+#    #+#             */
/*   Updated: 2025/12/14 21:17:43 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt_math.h"
#include "types.h"
#include <complex.h>
#include <math.h>
#include <bvh.h>

int	filter_real_numbers(int numvalues, complex double in[], double out[])
{
	int	num_real_values;
	int	i;

	num_real_values = 0;
	i = 0;
	while (i < numvalues)
	{
		if (fabs(cimag(in[i])) < ZEROD)
			out[num_real_values++] = creal(in[i]);
		i += 1;
	}
	return (num_real_values);
}

FORCEINLINE
static inline int	cis_zero(complex double x)
{
	return ((fabs(creal(x)) < ZEROD) && (fabs(cimag(x)) < ZEROD));
}

int	csolve_quad(t_cequ arg, complex double roots[])
{
	complex double	disc;
	complex double	r;
	complex double	d;

	if (cis_zero(arg.a))
	{
		if (cis_zero(arg.b))
			return (0);
		roots[0] = -arg.c / arg.b;
		return (1);
	}
	else
	{
		disc = arg.b * arg.b - 4.0 * arg.a * arg.c;
		if (cis_zero(disc))
		{
			roots[0] = -arg.b / (2.0 * arg.a);
			return (1);
		}
		r = csqrt(disc);
		d = 2.0 * arg.a;
		roots[0] = (-arg.b + r) / d;
		roots[1] = (-arg.b - r) / d;
		return (2);
	}
}

// Thanks Euler for this
complex double	ft_ccbrt(complex double a, int n)
{
	const double	twopi = 2.0 * 3.141592653589793238462643383279502884;
	double		rho;
	double		theta;

	rho = cabs(a);
	theta = carg(a);
	rho = pow(rho, (1.0 / 3.0));
	theta = (theta + (twopi * n)) * (1.0 / 3.0);
	return (rho * (cos(theta) + I * sin(theta)));
}

int	csolve_cubic(t_cequ arg, complex double roots[])
{
	complex double	s;
	complex double	d;
	complex double	e;
	complex double	froot;
	complex double	f;
	complex double	g;
	complex double	inv_a;
	int				i;

	if (cis_zero(arg.a))
		return (csolve_quad((t_cequ){arg.b, arg.c, arg.d, 0, 0}, roots));
	inv_a = 1 / arg.a;
	arg.b *= inv_a;
	arg.c *= inv_a;
	arg.d *= inv_a;
	s = arg.b * (1.0 / 3.0);
	d = arg.c * (1.0 / 3.0) - s * s;
	e = s * s * s + (arg.d - s * arg.c) * 0.5;
	froot = csqrt(e * e + d * d * d);
	f = -froot - e;
	if (cis_zero(f))
		f = froot - e;
	i = 0;
	while (i < 3)
	{
		g = ft_ccbrt(f, i);
		roots[i] = g - d/g - s;
		i += 1;
	}
	return (3);
}

int	csolve_quartic(t_cequ arg, complex double roots[])
{
	complex double	inv_a;
	complex double	b2;
	complex double	b4;
	complex double	alpha;
	complex double	alpha2;
	complex double	beta;
	complex double	gamma;
	complex double	t;
	complex double	rad;
	complex double	r1;
	complex double	r2;
	complex double	p;
	complex double	q;
	complex double	r;
	complex double	u;
	complex double	y;
	complex double	w;

	if (cis_zero(arg.a))
		return (csolve_cubic((t_cequ){arg.b, arg.c, arg.d, arg.e, 0}, roots));
	inv_a = 1 / arg.a;
	arg.b *= inv_a;
	arg.c *= inv_a;
	arg.d *= inv_a;
	arg.e *= inv_a;
	b2 = arg.b * arg.b;
	b4 = b2 * b2;
	alpha = -(3.0 / 8.0) * b2 + arg.c;
	beta = b2 * arg.b * (1.0 / 8.0) - arg.b * arg.c * 0.5 + arg.d;
	gamma = -(3.0 / 256.0) * b4 + b2 * arg.c * (1.0 / 16.0) - arg.b * arg.d * (1.0 / 4.0) + arg.e;
	alpha2 = alpha * alpha;
	t = -arg.b * (1.0 / 4.0);
	if (cis_zero(beta))
	{
		rad = csqrt(alpha2 - 4.0 * gamma);
		r1 = csqrt((-alpha + rad) * 0.5);
		r2 = csqrt((-alpha - rad) * 0.5);
		roots[0] = t + r1;
		roots[1] = t - r1;
		roots[2] = t + r2;
		roots[3] = t - r2;
	}
	else
	{
		p = -(alpha2 * (1.0 / 12.0) + gamma);
		q = -alpha2 * alpha * (1.0 / 108.0) + alpha * gamma * (1.0 / 3.0) - beta * beta * (1.0 / 8.0);
		r = -q * 0.5 + csqrt(q * q * (1.0 / 4.0) + p * p * p * (1.0 / 27.0));
		u = ft_ccbrt(r, 0);
		y = -(5.0 / 6.0) * alpha + u;
		if (cis_zero(u))
			y -= ft_ccbrt(q, 0);
		else
			y -= p / (3.0 * u);
		w = csqrt(alpha + 2.0 * y);
		r1 = csqrt(-(3.0 * alpha + 2.0 * y + 2.0 * beta / w));
		r2 = csqrt(-(3.0 * alpha + 2.0 * y - 2.0 * beta / w));
		roots[0] = t + (w - r1) * 0.5;
		roots[1] = t + (w + r1) * 0.5;
		roots[2] = t + (-w - r2) * 0.5;
		roots[3] = t + (-w + r2) * 0.5;
	}
	return (4);
}
