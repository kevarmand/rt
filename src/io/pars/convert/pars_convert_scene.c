#include "scene.h"
#include "convert.h"
#include "io.h"
#include "error_codes.h"
#include <stdlib.h>
#include <string.h>

int	pars_convert_scene(t_scene_parsed *parsed, t_scene *scene)
{
	t_conv_ctx	cx;
	int			err;

	conv_count_all(parsed, &cx);
	conv_globals(parsed, scene);
	err = conv_camera(parsed, scene, &cx);
	if (err == SUCCESS)
		err = conv_light(parsed, scene, &cx);
	if (err == SUCCESS)
		err = conv_obj(parsed, scene, &cx);
	if (err == SUCCESS)
	{
		ft_lstclear(&(parsed)->objects, free);
		parsed->objects = NULL;
		ft_lstclear(&(parsed)->cameras, free);
		parsed->cameras = NULL;
		ft_lstclear(&(parsed)->lights, free);
		parsed->lights = NULL;
	}
	return (err);
}
