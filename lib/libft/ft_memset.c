/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 10:29:55 by kearmand          #+#    #+#             */
/*   Updated: 2025/05/13 15:45:14 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_types.h"

__attribute__((always_inline)) static inline uintptr_t
	__ft_memset_big(size_t len, uintptr_t s_addr, t_op word)
{
	while (len > 0)
	{
		((t_op *)s_addr)[0] = word;
		((t_op *)s_addr)[1] = word;
		((t_op *)s_addr)[2] = word;
		((t_op *)s_addr)[3] = word;
		((t_op *)s_addr)[4] = word;
		((t_op *)s_addr)[5] = word;
		((t_op *)s_addr)[6] = word;
		((t_op *)s_addr)[7] = word;
		s_addr += 8 * OPSIZ;
		len -= 1;
	}
	return (s_addr);
}

__attribute__((always_inline)) static inline uintptr_t
	__ft_memset_mid(size_t len, uintptr_t s_addr, t_op word)
{
	while (len > 0)
	{
		((t_op *)s_addr)[0] = word;
		s_addr += OPSIZ;
		len -= 1;
	}
	return (s_addr);
}

__attribute__((always_inline))
static inline t_op	__ft_fill_word(int c)
{
	t_op	word;

	word = (unsigned char) c;
	word |= word << 8;
	word |= word << 16;
	if (OPSIZ > 4)
		word |= (word << 16) << 16;
	return (word);
}

__attribute__((flatten))
void	*ft_memset(void *s, int c, size_t n)
{
	uintptr_t	s_addr;
	t_op		word;

	s_addr = (uintptr_t) s;
	if (n >= 8)
	{
		word = __ft_fill_word(c);
		while (s_addr % OPSIZ != 0)
		{
			((t_byte *)s_addr)[0] = c;
			s_addr += 1;
			n -= 1;
		}
		s_addr = __ft_memset_big(n / (OPSIZ * 8), s_addr, word);
		n %= OPSIZ * 8;
		s_addr = __ft_memset_mid(n / OPSIZ, s_addr, word);
		n %= OPSIZ;
	}
	while (n > 0)
	{
		((t_byte *)s_addr)[0] = c;
		s_addr += 1;
		n -= 1;
	}
	return (s);
}
/*#include <stddef.h> */
/**/
/*void	*ft_memset(void *s, int c, size_t n)*/
/*{*/
/*	unsigned char	*temp;*/
/*	unsigned char	m;*/
/*	size_t			i;*/
/**/
/*	temp = (unsigned char *)s;*/
/*	i = 0;*/
/*	m = (unsigned char)c;*/
/*	while (i < n)*/
/*	{*/
/*		temp[i] = m;*/
/*		i++;*/
/*	}*/
/*	return (s);*/
/*}*/
