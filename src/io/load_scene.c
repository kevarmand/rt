#include "io.h"
#include <stdio.h>
#include <errors.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "scene.h"
#include "pars/parsing_internal.h"

void print_scene2(const t_scene_parsed *scene);

int load_scene(const char *path, t_scene *out_scene)
{
	t_scene_parsed	parsed;
	int		status;

	(void)out_scene;
	init_parsed_scene(&parsed);
	status = pars_scene(path, &parsed);
	if (status == SUCCESS)
		print_scene2(&parsed);
	if (status == SUCCESS)
		status = build_scene(out_scene, &parsed);
	free_parsed_scene(&parsed);
	return (status);
}
