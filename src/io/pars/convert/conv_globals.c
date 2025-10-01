#include "scene.h"
#include "convert.h"
#include "io.h"
#include "error_codes.h"
#include <stdlib.h>
#include <string.h>
#include "color.h"

void	conv_globals(const t_scene_parsed *parsed, t_scene *scene)
{
	scene->resolution_width = parsed->globals.res_width;
	scene->resolution_height = parsed->globals.res_height;
	rgb8_to_linear_vec((int [3]){
		(int)(parsed->globals.color[0]),
		(int)(parsed->globals.color[1]),
		(int)(parsed->globals.color[2])},
		&scene->ambient_color);
	scene->ambient_color.x *= parsed->globals.brightness;
	scene->ambient_color.y *= parsed->globals.brightness;
	scene->ambient_color.z *= parsed->globals.brightness;
}
