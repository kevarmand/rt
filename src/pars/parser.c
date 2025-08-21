/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 16:16:15 by karmand           #+#    #+#             */
/*   Updated: 2025/08/04 13:32:57 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		parser_check(char **arr, t_data *data)
{
	if (arr[0] == 0 || arr[0][0] == '#')
		return (SUCCESS);
	if (!ft_strcmp("A", arr[0]))
		return (pars_a(arr, data));
	if (!ft_strcmp("R", arr[0]))
		return (pars_res(arr, data));
	if (!ft_strcmp("l", arr[0]))
		return (pars_light(arr, data));
	if (!ft_strcmp("c", arr[0]))
		return (pars_cam(arr, data));
	if (!ft_strcmp("pl", arr[0]))
		return (pars_pl(arr, data));
	if (!ft_strcmp("sp", arr[0]))
		return (pars_sp(arr, data));
	if (!ft_strcmp("cy", arr[0]))
		return (pars_cy(arr, data));
	if (!ft_strcmp("tr", arr[0]))
		return (pars_tr(arr, data));
	if (!ft_strcmp("sq", arr[0]))
		return (pars_sq(arr, data));
	return (ERR_PARS_TYPE);
}

int		parsing(int fd, t_data *data)
{
	char	*line;
	char	**arr;
	int		ret;
	int		n;
	int		err;

	n = 0;
	ret = 1;
	while (ret == 1)
	{
		ret = get_next_line(fd, &line);
		if (ret == -1)
			return (1 + error(ERR_MALLOC));
		arr = ft_split(line, " \t");
		if (arr == 0)
			return (ERR_MALLOC + ft_free(line));
		if ((err = parser_check(arr, data)))
			return (10 + err2(err, line, n) + ft_free(line) + free_split(arr));
		free(line);
		free_split(arr);
		n++;
	}
	return (SUCCESS);
}
