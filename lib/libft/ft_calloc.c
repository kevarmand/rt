/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 11:02:33 by kearmand          #+#    #+#             */
/*   Updated: 2024/11/11 17:10:05 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

void	*ft_calloc(size_t n, size_t size)
{
	void	*ret;

	if (n == 0)
		return (0);
	if (n * size / n != size)
		return (0);
	ret = malloc(n * size);
	if (ret == 0)
		return (0);
	ft_bzero(ret, n * size);
	return (ret);
}
