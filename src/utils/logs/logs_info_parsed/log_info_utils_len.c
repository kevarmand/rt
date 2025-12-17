/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log_info_utils_len.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 03:19:28 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/17 03:39:00 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "log_info_internal.h"

void	put_nchar(char ch, int count)
{
	int	index;

	index = 0;
	while (index < count)
	{
		ft_putchar_fd(ch, 1);
		index++;
	}
}

void	print_edge(const char *l, const char *h, const char *r)
{
	ft_putstr_fd(C_FRAME, 1);
	ft_putstr_fd(l, 1);
	put_nstr(h, LOG_INFO_W - 2);
	ft_putstr_fd(r, 1);
	ft_putstr_fd(LOG_RESET, 1);
	ft_putchar_fd('\n', 1);
}

void	print_empty_line(void)
{
	ft_putstr_fd(C_FRAME, 1);
	ft_putstr_fd(UTF8_VLINE, 1);
	put_nchar(' ', LOG_INFO_W - 2);
	ft_putstr_fd(UTF8_VLINE, 1);
	ft_putstr_fd(LOG_RESET, 1);
	ft_putchar_fd('\n', 1);
}

int	inner_width(void)
{
	return (LOG_INFO_W - 2);
}

int	col_left_width(void)
{
	return (inner_width() / 2);
}
