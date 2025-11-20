/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 16:18:45 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/19 16:19:34 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "errors.h"

int	check_args(int argc, char **argv)
{
	char	*filename;
	size_t	len;

	if (argc != 2)
	{
		print_error("Usage: ./rt <scene_file>", ERR_INVALID_ARGS);
		return (ERR_INVALID_ARGS);
	}
	filename = argv[1];
	len = strlen(filename);
	if (len < 4 || strcmp(filename + len - 3, ".rt") != 0)
	{
		print_error("Scene file must have a .rt extension",  ERR_INVALID_ARGS);
		return (ERR_INVALID_ARGS);
	}
	return (SUCCESS);
}
