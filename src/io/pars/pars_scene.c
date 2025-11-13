#include "io.h"
#include "type.h"
#include "libft.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "error_codes.h"
#include "convert.h"



int	pars_scene(const char *filename, t_conv_ctx *cx)
{
	int				fd;
	int				err;
	t_scene_parsed	scene_parsed;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (ERR_OPEN);
	pars_init_scene(&scene_parsed);
	err = pars_scene_fd(fd, &scene_parsed);
	close(fd);
	if (err == SUCCESS)
		err = convert_parsed_to_ctx(&scene_parsed, cx);
	free_parsed_scene(&scene_parsed);
	return (err);
}
