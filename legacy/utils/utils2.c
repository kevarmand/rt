/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmand <karmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/10 15:54:19 by karmand           #+#    #+#             */
/*   Updated: 2020/01/11 17:18:44 by karmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		ft_strlen(char *str)
{
	int		n;

	n = 0;
	while (str[n])
		n++;
	return (n);
}

int		ft_free(void *ptr)
{
	free(ptr);
	return (0);
}

int		split_count(char **arr)
{
	int i;

	i = 0;
	while (arr[i])
		i++;
	return (i);
}
