/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmand <karmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 16:16:15 by karmand           #+#    #+#             */
/*   Updated: 2020/03/05 17:09:10 by karmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int			ft_count(char **arr, int size_sep)
{
	int		i;
	int		n;

	n = -size_sep;
	i = 0;
	while (arr[i])
	{
		n += ft_strlen(arr[i]);
		i++;
		n += size_sep;
	}
	if (n < 0)
		return (0);
	return (n);
}

char		*ft_strjoin(char **arr, char *sep)
{
	char	*res;
	int		i;
	int		j;
	int		n;

	if (!(res = malloc(1 + ft_count(arr, ft_strlen(sep)))))
		return (0);
	i = -1;
	j = 0;
	n = 0;
	while (arr[++i])
	{
		j = 0;
		while (arr[i][j])
			res[n++] = arr[i][j++];
		j = 0;
		if (arr[i + 1] != 0)
			while (sep[j])
				res[n++] = sep[j++];
	}
	res[n] = 0;
	return (res);
}
