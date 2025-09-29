#include "io.h"
#include "type.h"
#include "libft.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "error_codes.h"


int	pars_scene(const char *filename, t_scene *scene)
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
	// if (err == SUCCESS)
	// 	err = pars_check_scene(scene_parsed);
	// if (err == SUCCESS)
	// 	err = pars_convert_scene(scene_parsed, scene);
	// pars_cleanup_scene(scene_parsed);
	(void)scene;
	return (err);
}
