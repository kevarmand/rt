/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 16:18:45 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 06:10:05 by kearmand         ###   ########.fr       */
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
		perr(ERR_INTERNAL, "Usage: ./rt <scene.rt>");
		return (ERR_INTERNAL);
	}
	filename = argv[1];
	len = ft_strlen(filename);
	if (len < 4 || ft_strcmp(filename + len - 3, ".rt") != 0)
	{
		perr(ERR_INTERNAL, "Scene file must have a .rt extension");
		return (ERR_INTERNAL);
	}
	return (SUCCESS);
}
