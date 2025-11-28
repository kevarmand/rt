/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmand <karmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 16:16:15 by karmand           #+#    #+#             */
/*   Updated: 2020/03/05 17:09:10 by karmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

int		is_wp(char c, char *wp)
{
	while (*wp)
	{
		if (*wp == c)
			return (1);
		wp++;
	}
	return (0);
}

int		count_word(char *str, char *wp)
{
	int	n;

	n = 0;
	while (*str && is_wp(*str, wp))
		str++;
	while (*str)
	{
		n++;
		while (*str && !is_wp(*str, wp))
			str++;
		while (*str && is_wp(*str, wp))
			str++;
	}
	return (n);
}

char	*malloc_word(char *str, char *wp)
{
	char	*word;
	int		i;

	i = 0;
	while (str[i] && !is_wp(str[i], wp))
		i++;
	word = malloc(i + 1);
	word[i] = '\0';
	while (--i >= 0)
		word[i] = str[i];
	return (word);
}

char	**ft_split(char *str, char *wp)
{
	char	**ret;
	int		i;

	ret = malloc(sizeof(char *) * (count_word(str, wp) + 1));
	i = 0;
	while (*str && is_wp(*str, wp))
		str++;
	while (*str)
	{
		ret[i] = malloc_word(str, wp);
		i++;
		while (*str && !is_wp(*str, wp))
			str++;
		while (*str && is_wp(*str, wp))
			str++;
	}
	ret[i] = NULL;
	return (ret);
}

int		free_split(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
	return (0);
}
