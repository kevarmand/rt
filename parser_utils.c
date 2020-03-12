/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmand <karmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/10 16:08:11 by karmand           #+#    #+#             */
/*   Updated: 2020/02/20 17:03:11 by karmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

char	get_int(int fd, int *n)
{
	char	c;
	int		r;
	int		sign;
	int		i;
	
	i = 0;
	*n = 0;
	sign  = 1;
	r = read(fd, &c, 1);
	while (r && ft_iswp(c))
		r = read(fd, &c, 1);
	if (c == '-')
	{
		sign = -1;
		r = read(fd, &c, 1);
	}
	while (r && ft_isdigit(c) && ++i < 8)
	{
		(*n) = 10 * (*n) + (c - '0');
		r = read(fd, &c, 1);
	}
	if (i == 8)
		c = 0;
	(*n) = sign * (*n) + sign - 1;
	printf("{n=%i}", *n);
	return (c);
}

char	get_double(int fd, double *d)
{
	int n;
	int r;
	char c;
	int sign;

	n = 0;
	c = get_int(fd, &n);

	sign = 1;
	if (n < 0)
	{
		sign = -1;
		n += 2;
	}
	(*d) = (double)n;
	if (c != '.')
		return (c);
	n = 10;
	r = read(fd, &c, 1);
	while (r && ft_isdigit(c))
	{
		(*d) += sign * ((double)(c - '0'))/(double)n;
		n *= 10;
		r = read(fd, &c, 1);
	}
	return (c);
}
