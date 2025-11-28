/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_gnl.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmand <karmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 14:26:30 by karmand           #+#    #+#             */
/*   Updated: 2020/02/19 14:51:01 by karmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>

int		gnl_rec(int fd, char **line, int n)
{
	char	c;
	int		err;

	err = read(fd, &c, 1);
	if (err == -1)
		return (-1);
	if (err == 0 || c == '\n')
	{
		if (!(*line = malloc(n + 1)))
			return (-1);
		(*line)[n] = '\0';
		return (err);
	}
	err = gnl_rec(fd, line, n + 1);
	if (err == -1)
		return (-1);
	(*line)[n] = c;
	return (err);
}

int		get_next_line(int fd, char **line)
{
	return (gnl_rec(fd, line, 0));
}
