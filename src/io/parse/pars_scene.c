/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_scene.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 16:50:27 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/18 21:32:52 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include "libft.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "errors.h"
#include "parsing_internal.h"

static void	pars_state_init(t_pars_state *st)
{
	st->line_no = 0;
	st->depth = 0;
	st->line = NULL;
	st->pos = 0;
}

/**
 * @brief Parse a scene from an open file descriptor into a temporary builder.
 * @param fd    Open file descriptor to the .rt scene (read-only). (no_null)
 * @param scene Builder receiving parsed elements (lists, presence mask).
 * (no_null)
 * @return SUCCESS on success, otherwise a parsing/I-O error code.
 * @note Does not close @fd. Preconditions are guaranteed by the caller.
 */
static int	pars_scene_fd(int fd, t_scene_parsed *scene)
{
	char			*line;
	int				err;
	t_pars_state	state;

	err = SUCCESS;
	pars_state_init(&state);
	while (!err)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		state.line_no++;
		err = pars_line(line, scene, &state);
		free(line);
		if (err)
			return (err);
	}
	if (state.depth != 0)
		return (ERR_CTX_UNBALANCED);
	return (SUCCESS);
}

int	pars_scene(const char *filename, t_scene_parsed *scene)
{
	int				fd;
	int				err;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (ERR_OPEN);
	err = pars_scene_fd(fd, scene);
	close(fd);
	return (err);
}
