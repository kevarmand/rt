# include "io.h"
# include "error_codes.h"


int	pars_init_scene(t_scene_parsed *scene)
{
	scene->cameras = NULL;
	scene->lights = NULL;
	scene->objects = NULL;
	scene->globals.res_height = 600;
	scene->globals.res_width = 800;
	scene->globals.brightness = 0.1;
	scene->globals.color.r = 255;
	scene->globals.color.g = 255;
	scene->globals.color.b = 255;
	scene->presence_mask = 0;
	return (SUCCESS);
}
