/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmand <karmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/10 15:54:19 by karmand           #+#    #+#             */
/*   Updated: 2020/01/11 17:18:44 by karmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

double	ft_abs(double a)
{
	if (a < 0)
		return (-a);
	return (a);
}

void	ft_bzero(void *s, int n)
{
	char	*str;
	int		i;

	str = (char *)s;
	i = 0;
	while (i < n)
		str[i++] = 0;
}

int		ft_strcmp(char *s1, char *s2)
{
	int i;

	i = 0;
	while (s1[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

int		ft_isdigit(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int		ft_iswp(char c, char *wp)
{
	while (*wp)
	{
		if (c == *wp)
			return (1);
		wp++;
	}
	return (0);
}
