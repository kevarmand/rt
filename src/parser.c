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
//	if (!ft_strcmp("cy", arr[0]))
//		return (pars_cy(arr, data));
	if (!ft_strcmp("tr", arr[0]))
		return (pars_tr(arr, data));
	return (ERROR_PARS_TYPE);
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
			return (error(ERR_MALLOC));
		arr = ft_split(line, " \t");
		if (arr == 0)
			return (ERR_MALLOC + free(line));
		if ((err = parser_check(arr, data)))
			return (err2(err, line, n) + free(line) + free_split(arr));
		free(line);
		free_split(arr);
		n++;
	}
	return (SUCCESS);
}
