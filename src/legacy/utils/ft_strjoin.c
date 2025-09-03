/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 16:16:15 by karmand           #+#    #+#             */
/*   Updated: 2025/08/04 14:21:59 by kearmand         ###   ########.fr       */
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

char	*ft_strjoin2(const char *s1, const char *s2)
{
	char	*res;
	int		i;
	int		j;

	if (!s1 || !s2)
		return (NULL);
	res = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!res)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		res[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
	{
		res[i + j] = s2[j];
		j++;
	}
	res[i + j] = '\0';
	return (res);
}