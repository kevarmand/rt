/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_scene.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 16:50:27 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 08:43:58 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include "libft.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "errors.h"
#include "parsing_internal.h"
#include "line_reader.h"

static int	pars_reader(t_line_reader *reader, t_scene_parsed *scene)
{
	t_pars_state	state;
	char			*line;
	size_t			len;
	int				status;

	state = (t_pars_state){0};
	status = line_reader_next(reader, &line, &len);
	while (status == 1)
	{
		state.line_no++;
		if (ft_memchr(line, '\0', len))
			status = perr(ERR_PARS, "Unexpected NUL byte in scene file.");
		else
			status = pars_line(line, scene, &state);
		if (status != SUCCESS)
			return (perr_line(status, line, state.line_no));
		status = line_reader_next(reader, &line, &len);
	}
	if (status == ERR_READ)
		return (perr(status, "Failed to read scene file."));
	if (status == ERR_MALLOC)
		return (perr(status, PERR_MALLOC));
	if (status == ERR_PARS)
		return (perr_line(status, "Line exceeds parser index range.",
				state.line_no + 1));
	return (status);
}

int	pars_scene(const char *filename, t_scene_parsed *scene)
{
	t_line_reader	reader;
	int				fd;
	int				err;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (perr(ERR_OPEN, PERR_OPEN));
	err = line_reader_init(&reader, fd);
	if (err == SUCCESS)
		err = pars_reader(&reader, scene);
	else
		perr(err, PERR_MALLOC);
	line_reader_destroy(&reader);
	close(fd);
	return (err);
}
