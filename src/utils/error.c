/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 14:52:55 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 06:08:59 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "errors.h"
#include "libft.h"

#define RED "\033[0;31m"
#define RESET "\033[0m"
#define YELLOW "\033[0;33m"

int	perr(int err, const char *msg)
{
	ft_putstr_fd(RED "Error" RESET ": ", 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("\n", 2);
	return (err);
}

int	perr_line(int err, const char *line, int line_no)
{
	ft_putstr_fd(RED "Error" RESET , 2);
	ft_putstr_fd("Incorrect token at line : 	", 2);
	ft_putnbr_fd(line_no, 2);
	ft_putstr_fd("\n" YELLOW, 2);
	ft_putstr_fd(line, 2);
	ft_putstr_fd(RESET "\n", 2);
	return (err);
}
