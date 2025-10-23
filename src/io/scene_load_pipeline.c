#include "io.h"
#include "rt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "scene.h"
#include "convert.h"

void init_ctx_parsing(t_conv_ctx *cx)
{
	ft_memset(cx, 0, sizeof(t_conv_ctx));
	vector_init(&cx->tex_v, 16, sizeof(t_texture));
	hashmap_init(&cx->tex_m, 16, sizeof(t_index));
	vector_init(&cx->mat_v, 16, sizeof(t_material));
	hashmap_init(&cx->mat_m, 16, sizeof(t_index));
	vector_init(&cx->surf_v, 16, sizeof(t_surface));
	vector_init(&cx->cam_v, 8, sizeof(t_camera));
	vector_init(&cx->light_v, 8, sizeof(t_light));
	vector_init(&cx->plane_v, 16, sizeof(t_primitive));
	vector_init(&cx->obj_v, 16, sizeof(t_object));
}

int scene_load_pipeline(const char *path, t_scene *out_scene)
{
	t_conv_ctx	cx;
	int			status;

	init_ctx_parsing(&cx);
	status = parse_scene_file(path, &cx);
	if (status == SUCCESS)
		status = build_scene_from_ctx(out_scene, &cx);
	if (status == SUCCESS)
		status = finalize_scene(out_scene);
	destroy_ctx_parsing(&cx);
	return status;
}
