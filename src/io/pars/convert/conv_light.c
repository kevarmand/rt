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

static void	copy_data_light(const t_parsed_light *src, t_light *dst)
{
	vec3f_from_float3(src->position, &dst->position);
	rgb8_to_linear_vec(src->rgb, &dst->color);
	dst->color.x *= src->brightness;
	dst->color.y *= src->brightness;
	dst->color.z *= src->brightness;
}

int	conv_light(const t_scene_parsed *parsed, t_scene *scene, t_conv_ctx *cx)
{
	t_list	*lst;
	int		i;

	scene->light_count = cx->light_count;
	scene->lights = malloc(sizeof(t_light) * scene->light_count);
	if (!scene->lights)
		return (ERR_MALLOC);
	ft_memset(scene->lights, 0, sizeof(t_light) * scene->light_count);
	lst = parsed->lights;
	i = 0;
	while (lst)
	{
		copy_data_light(lst->content, &scene->lights[i]);
		lst = lst->next;
		i++;
	}
	return (SUCCESS);
}
