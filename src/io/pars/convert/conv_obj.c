#include "color.h"
#include "scene.h"
#include "convert.h"
#include "io.h"
#include "error_codes.h"
#include <stdlib.h>


static int	init_obj_plane_alloc(t_scene *scene, t_conv_ctx *cx)
{
	scene->object_count = cx->object_count;
	scene->objects = malloc(sizeof(t_object) * scene->object_count);
	if (!scene->objects)
		return (ERR_MALLOC);
	ft_memset(scene->objects, 0, sizeof(t_object) * scene->object_count);
	scene->plane_count = cx->plane_count;
	scene->planes = malloc(sizeof(t_plane) * scene->plane_count);
	if (!scene->planes)
		return (ERR_MALLOC);
	ft_memset(scene->planes, 0, sizeof(t_plane) * scene->plane_count);
	return (SUCCESS);
}

static void	copy_plane_data(const t_parsed_element *src, t_plane *dst)
{
	dst->normal.x = src->data.plane.normal[0];
	dst->normal.y = src->data.plane.normal[1];
	dst->normal.z = src->data.plane.normal[2];
	dst->d = -(dst->normal.x * src->data.plane.origin[0]
		+ dst->normal.y * src->data.plane.origin[1]
		+ dst->normal.z * src->data.plane.origin[2]);
}

static void	copy_object_data(const t_parsed_element *src,
	t_object *dst, const t_scene_parsed *parsed, t_conv_ctx *cx)
{
	
}

int	conv_obj(t_scene_parsed *parsed, t_scene *scene, t_conv_ctx *cx)
{
	t_list				*lst;
	int					i;
	int					j;
	t_parsed_element	*elem;

	if (init_obj_plane_alloc(scene, cx))
		return (ERR_MALLOC);
	i = 0;
	j = 0;
	lst = parsed->objects;
	while (lst)
	{
		elem = lst->content;
		if (elem->type == ELEM_PLANE)
			copy_plane_data(elem, &scene->planes[i++]);
		else
			copy_object_data(lst->content, &scene->objects[j++], parsed, cx);
		lst = lst->next;
	}
	return (SUCCESS);
}
