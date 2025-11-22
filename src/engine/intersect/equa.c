/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   equa.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 18:36:20 by karmand           #+#    #+#             */
/*   Updated: 2025/11/22 14:39:37 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

double	equa_premier(double a, double b)
{
	return (-b / a);
}

double	equa_second(double a, double b, double c)
{
	double	d;
	double	x1;
	double	x2;

	/* Cas dégénéré : 0·x² + 0·x + c = 0 */
	if (a == 0.0 && b == 0.0)
		return (-1.0);

	/* Cas linéaire : b·x + c = 0 */
	if (a == 0.0)
	{
		x1 = -c / b;
		return (x1 > 0.0 ? x1 : -1.0);
	}

	d = b * b - 4.0 * a * c;
	if (d < 0.0)
		return (-1.0);
	d = sqrt(d);

	x1 = (-b - d) / (2.0 * a);
	x2 = (-b + d) / (2.0 * a);

	if (x1 > 0.0 && x2 > 0.0)
		return (x1 < x2 ? x1 : x2);
	if (x1 > 0.0)
		return (x1);
	if (x2 > 0.0)
		return (x2);
	return (-1.0);
}
