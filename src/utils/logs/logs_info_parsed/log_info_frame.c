/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log_info_frame.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 03:20:31 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/17 03:30:32 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "log_info_internal.h"

int	col_right_width(void)
{
	return (inner_width() - col_left_width());
}

void	print_line_cols_begin(void)
{
	ft_putstr_fd(C_FRAME, 1);
	ft_putstr_fd(UTF8_VLINE, 1);
	ft_putstr_fd(LOG_RESET, 1);
}

void	print_line_cols_end(void)
{
	ft_putstr_fd(C_FRAME, 1);
	ft_putstr_fd(UTF8_VLINE, 1);
	ft_putstr_fd(LOG_RESET, 1);
	ft_putchar_fd('\n', 1);
}

void	print_col_center_prefix(int col_w, int content_len)
{
	int	pad;

	pad = (col_w - content_len) / 2;
	if (pad < 0)
		pad = 0;
	put_nchar(' ', pad);
}

void	print_col_center_suffix(int col_w, int content_len)
{
	int	pad_left;
	int	pad_right;

	pad_left = (col_w - content_len) / 2;
	if (pad_left < 0)
		pad_left = 0;
	pad_right = col_w - pad_left - content_len;
	if (pad_right < 0)
		pad_right = 0;
	put_nchar(' ', pad_right);
}
