/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmand <karmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/29 15:00:38 by karmand           #+#    #+#             */
/*   Updated: 2020/02/19 15:00:06 by karmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_buffer		*buf_create(int size)
{
	t_buffer	*buf;

	if (!(buf = malloc(sizeof(t_buffer))))
		return (0);
	if (!(buf->buff = malloc(size * sizeof(char))))
	{
		free(buf);
		return (0);
	}
	buf->cur = 0;
	buf->size = size;
	return (buf);
}

int				buf_dest(t_buffer *b)
{
	free(b->buff);
	free(b);
	return (0);
}

int				buf_addchar(t_buffer *b, char c)
{
	int		i;
	char	*s;

	if (b->cur == b->size)
	{
		if (!(s = malloc(sizeof(char) * 2 * b->size)))
			return (ERROR_MALLOC);
		i = -1;
		while (++i < b->size)
			s[i] = (b->buff)[i];
		free(b->buff);
		b->buff = s;
		b->size *= 2;
	}
	(b->buff)[b->cur] = c;
	(b->cur)++;
	return (0);
}

char			*buf_to_string(t_buffer *b)
{
	char	*s;
	int		i;

	if (!(s = malloc(sizeof(char) * (1 + b->cur))))
		return (0);
	i = -1;
	while (++i < b->cur)
		s[i] = (b->buff)[i];
	s[i] = 0;
	return (s);
}
