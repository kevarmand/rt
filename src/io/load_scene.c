#include "io.h"
#include <stdio.h>
#include <errors.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "scene.h"

int load_scene(const char *path, t_scene *out_scene)
{
	t_scene_parsed	parsed;
	int		status;

	init_parsed_scene(&parsed);
	status = parse_scene(path, &parsed);
	if (status == SUCCESS)
		status = build_scene(&parsed, out_scene);
	free_parsed_scene(&parsed);
	return (status);
}
