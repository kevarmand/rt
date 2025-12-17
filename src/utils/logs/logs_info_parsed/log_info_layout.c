/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log_info_layout.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 03:20:07 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/17 03:39:11 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "log_info_internal.h"

void	print_line_left_begin(void)
{
	ft_putstr_fd(C_FRAME, 1);
	ft_putstr_fd(UTF8_VLINE, 1);
	ft_putstr_fd(" ", 1);
	ft_putstr_fd(LOG_RESET, 1);
}

void	print_line_left_end(int content_len)
{
	int	pad;

	pad = (inner_width() - 1) - content_len;
	if (pad < 0)
		pad = 0;
	put_nchar(' ', pad);
	ft_putstr_fd(C_FRAME, 1);
	ft_putstr_fd(UTF8_VLINE, 1);
	ft_putstr_fd(LOG_RESET, 1);
	ft_putchar_fd('\n', 1);
}

void	print_centered_prefix(int visible_len)
{
	int	avail;
	int	pad_left;

	avail = LOG_INFO_W - 2;
	pad_left = (avail - visible_len) / 2;
	if (pad_left < 0)
		pad_left = 0;
	ft_putstr_fd(C_FRAME, 1);
	ft_putstr_fd(UTF8_VLINE, 1);
	put_nchar(' ', pad_left);
	ft_putstr_fd(LOG_RESET, 1);
}

void	print_centered_suffix(int visible_len)
{
	int	avail;
	int	pad_left;
	int	pad_right;

	avail = LOG_INFO_W - 2;
	pad_left = (avail - visible_len) / 2;
	if (pad_left < 0)
		pad_left = 0;
	pad_right = avail - pad_left - visible_len;
	if (pad_right < 0)
		pad_right = 0;
	put_nchar(' ', pad_right);
	ft_putstr_fd(C_FRAME, 1);
	ft_putstr_fd(UTF8_VLINE, 1);
	ft_putstr_fd(LOG_RESET, 1);
	ft_putchar_fd('\n', 1);
}

void	print_inner_hline(void)
{
	ft_putstr_fd(C_FRAME, 1);
	ft_putstr_fd(UTF8_VLINE, 1);
	put_nstr(UTF8_HLINE, LOG_INFO_W - 2);
	ft_putstr_fd(UTF8_VLINE, 1);
	ft_putstr_fd(LOG_RESET, 1);
	ft_putchar_fd('\n', 1);
}
