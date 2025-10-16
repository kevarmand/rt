#include "scene.h"
#include "convert.h"
#include "io.h"
#include "libft.h"
#include "error_codes.h"
#include <stdlib.h>
#include <string.h>

static void vec3f_from_float3(const float src[3], t_vec3f *dst)
{
	dst->x = src[0];
	dst->y = src[1];
	dst->z = src[2];
}

static void	cam_from_parsed(const t_parsed_camera *src, t_camera *dst)
{
	vec3f_from_float3(src->position, &dst->origin);
	vec3f_from_float3(src->orientation, &dst->forward);
	dst->fov_deg = src->fov;
}
int	conv_cameras_to_ctx(const t_scene_parsed *parsed, t_conv_ctx *cx)
{
	t_list		*list_node;
	int			index_camera;
	t_camera	camera_tmp;

	if (vector_reserve(&cx->cam_v, cx->camera_count) != SUCCESS)
		return (ERR_MALLOC);
	list_node = parsed->cameras;
	index_camera = 0;
	while (list_node)
	{
		cam_from_parsed(list_node->content, &camera_tmp);
		if (vector_push_back(&cx->cam_v, &camera_tmp) != SUCCESS)
			return (ERR_MALLOC);
		list_node = list_node->next;
		index_camera++;
	}
	return (SUCCESS);
}

/* Préconditions:
   - cx->cam_v contient exactement cx->camera_count éléments de type t_camera
   - scene est vierge côté cameras
*/

int	finalize_cameras(const t_conv_ctx *cx, t_scene *scene)
{
	const int	count = vector_size(&cx->cam_v);
	const void	*src_ptr = vector_data(&cx->cam_v);

	scene->camera_count = count;
	scene->cameras = malloc(sizeof(t_camera) * count);
	if (!scene->cameras)
		return (ERR_MALLOC);
	ft_memcpy(scene->cameras, src_ptr, sizeof(t_camera) * count);
	return (SUCCESS);
}
