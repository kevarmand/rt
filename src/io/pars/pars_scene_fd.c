#include "error_codes.h"
#include "type.h"
#include "libft.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "io.h"

int	pars_scene_fd(int fd, t_scene_parsed *scene)
{
	char	*line;
	int		err;
	t_pars_state	state;

	err = SUCCESS;
	pars_ctx_init(&state);
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

