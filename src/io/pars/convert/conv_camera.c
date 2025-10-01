#include "scene.h"
#include "convert.h"
#include "io.h"
#include "error_codes.h"
#include <stdlib.h>
#include <string.h>

static void vec3f_from_float3(const float src[3], t_vec3f *dst)
{
	dst->x = src[0];
	dst->y = src[1];
	dst->z = src[2];
}

static void	copy_data_camera(const t_parsed_camera *src, t_camera *dst)
{
	vec3f_from_float3(src->position, &dst->origin);
	vec3f_from_float3(src->orientation, &dst->forward);
	dst->fov_deg = src->fov;
}

int	conv_camera(const t_scene_parsed *parsed, t_scene *scene, t_conv_ctx *cx)
{
	t_list	*lst;
	int		i;
	t_camera	*cam;

	scene->camera_count = cx->camera_count;
	scene->cameras = malloc(sizeof(t_camera) * scene->camera_count);
	if (!scene->cameras)
		return (ERR_MALLOC);
	ft_memset(scene->cameras, 0, sizeof(t_camera) * scene->camera_count);
	lst = parsed->cameras;
	i = 0;
	while (lst)
	{
		copy_data_camera(lst->content, &scene->cameras[i]);
		lst = lst->next;
		i++;
	}	
	return (SUCCESS);
}
