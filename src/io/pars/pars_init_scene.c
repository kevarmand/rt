# include "io.h"
# include "error_codes.h"

int	pars_init_scene(t_scene_parsed *scene)
{
	scene->cameras = NULL;
	scene->lights = NULL;
	scene->objects = NULL;
	scene->globals.res_height = 600;
	scene->globals.res_width = 800;
	scene->globals.brightness = 0.4f;
	scene->globals.color[0] = 0.0f;
	scene->globals.color[1] = 0.0f;
	scene->globals.color[2] = 0.0f;
	scene->presence_mask = 0;
	return (SUCCESS);
}
