#include "scene.h"
#include "convert.h"
#include "io.h"
#include "errors.h"
#include <stdlib.h>
#include <string.h>
#include "color.h"
#include "vector.h"


void light_from_parsed(t_parsed_element *src, t_light *dst)
{
	vec3f_load3(&dst->position, src->data.light.position);
	rgb8_to_linear_vec(src->data.light.rgb, &dst->color);
	dst->intensity = src->data.light.brightness;
}
static void	reset_light(t_light *light)
{
	memset(light, 0, sizeof(t_light));
}

int	conv_lights(t_scene_parsed *parsed, t_conv_ctx *cx)
{
	t_list		*list_node;
	t_light		light_tmp;

	if (vector_reserve(&cx->light_v, cx->light_count) != SUCCESS)
		return (ERR_MALLOC);
	list_node = parsed->lights;
	while (list_node)
	{
		reset_light(&light_tmp);
		light_from_parsed(list_node->content, &light_tmp);
		if (vector_push_back(&cx->light_v, &light_tmp) != SUCCESS)
			return (ERR_MALLOC);
		list_node = list_node->next;
	}
	return (SUCCESS);
}
