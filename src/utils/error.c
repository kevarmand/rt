/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 14:52:55 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/21 14:15:55 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "errors.h"
#include "libft.h"

int	print_error(int err, const char *msg)
{
	ft_putstr_fd("Error : ", 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("\n", 2);
	return (err);
}

int	error_at(int err, const char *file, int line)
{
	ft_putstr_fd("Error at ", 2);
	ft_putstr_fd(file, 2);
	ft_putstr_fd(":", 2);
	ft_putnbr_fd(line, 2);
	ft_putstr_fd("\n", 2);
	return (err);
}
