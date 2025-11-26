/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 16:18:45 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/21 14:15:37 by kearmand         ###   ########.fr       */
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
		print_error(ERR_INVALID_ARGS, "Usage: ./rt <scene_file>");
		return (ERR_INVALID_ARGS);
	}
	filename = argv[1];
	len = ft_strlen(filename);
	if (len < 4 || ft_strcmp(filename + len - 3, ".rt") != 0)
	{
		print_error(ERR_INVALID_ARGS, "Scene file must have a .rt extension");
		return (ERR_INVALID_ARGS);
	}
	return (SUCCESS);
}
