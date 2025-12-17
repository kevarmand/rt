/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 03:32:05 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/17 03:34:18 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "log_info_internal.h"

int	int_len(int value)
{
	int	len;

	if (value == 0)
		return (1);
	len = 0;
	if (value < 0)
	{
		len++;
		value = -value;
	}
	while (value > 0)
	{
		value /= 10;
		len++;
	}
	return (len);
}

void	put_nstr(const char *str, int count)
{
	int	index;

	index = 0;
	while (index < count)
	{
		ft_putstr_fd(str, 1);
		index++;
	}
}
