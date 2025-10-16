#include "scene.h"
#include "convert.h"
#include "io.h"
#include "error_codes.h"
#include <stdlib.h>
#include <string.h>
#include "color.h"

static void vec3f_from_float3(const float src[3], t_vec3f *dst)
{
	dst->x = src[0];
	dst->y = src[1];
	dst->z = src[2];
}


int	conv_lights_to_ctx(const t_scene_parsed *parsed, t_conv_ctx *cx)
{
	t_list		*list_node;
	t_light		light_tmp;

	if (vector_reserve(&cx->light_v, cx->light_count) != SUCCESS)
		return (ERR_MALLOC);
	list_node = parsed->lights;
	while (list_node)
	{
		light_from_parsed(list_node->content, &light_tmp);
		if (vector_push_back(&cx->light_v, &light_tmp) != SUCCESS)
			return (ERR_MALLOC);
		list_node = list_node->next;
	}
	return (SUCCESS);
}

/* Préconditions:
   - cx->light_v contient exactement cx->light_count éléments de type t_light
   - scene est vierge côté lights
*/

int	finalize_lights(const t_conv_ctx *cx, t_scene *scene)
{
	const int	count = vector_size(&cx->light_v);
	const void	*src_ptr = vector_data(&cx->light_v);

	scene->light_count = count;
	scene->lights = malloc(sizeof(t_light) * count);
	if (!scene->lights)
		return (ERR_MALLOC);
	ft_memcpy(scene->lights, src_ptr, sizeof(t_light) * count);
	return (SUCCESS);
}

