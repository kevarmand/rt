/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   equa.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmand <karmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 18:36:20 by karmand           #+#    #+#             */
/*   Updated: 2020/03/02 17:11:52 by karmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

double	equa_premier(double a, double b)
{
	return (-b / a);
}

double	equa_second(double a, double b, double c)
{
	double	d;
	double	x1;
	double	x2;

	d = b * b - 4 * a * c;
	if (d < 0)
		return (-1);
	d = sqrt(d);
	x1 = (-b - d) / (2 * a);
	x2 = (-b + d) / (2 * a);
	if (x1 < 0)
		return (x2);
	else if (x1 < x2)
		return (x1);
	return (x2);
}
