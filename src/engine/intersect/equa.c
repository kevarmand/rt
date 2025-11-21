/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   equa.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 18:36:20 by karmand           #+#    #+#             */
/*   Updated: 2025/11/21 22:18:45 by kearmand         ###   ########.fr       */
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

	d = b * b - 4 * a * c;
	if (a == 0)
		return (-c / b);
	if (d < 0)
		return (-1);
	d = sqrt(d);
	x1 = (-b - d) / (2 * a);
	x2 = (-b + d) / (2 * a);
	if (x1 < 0)
		return (x2);
	else if (x1 < x2 && x1 > 0.001)
		return (x1);
	return (x2);
}
