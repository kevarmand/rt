#include "color.h"
#include "scene.h"
#include "convert.h"
#include "io.h"
#include "error_codes.h"
#include <stdlib.h>

static int	init_primitives(t_conv_ctx *cx)
{
	if (vector_reserve(&cx->obj_v, cx->object_count) != SUCCESS)
		return (ERR_MALLOC);
	if (vector_reserve(&cx->plane_v, cx->plane_count) != SUCCESS)
		return (ERR_MALLOC);
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

static void	object_from_parsed(const t_parsed_element *src, t_primitive *dst)
{
	if (src->type == ELEM_TRIANGLE)
		copy_triangle_to_primitive(src, dst);
	else if (src->type == ELEM_SPHERE)
		copy_sphere_to_primitive(src, dst);
	else if (src->type == ELEM_CYLINDER)
		copy_cylinder_to_primitive(src, dst);
	else if (src->type == ELEM_PLANE)
		copy_plane_to_primitive(src, dst);
}
	

static int	conv_push_elem_to_ctx(const t_parsed_element *elem, t_conv_ctx *cx)
{
	t_primitive		prim_tmp;

	object_from_parsed(elem, &prim_tmp);
	conv_option_primitive(&prim_tmp, elem->options);
	if (elem->type != ELEM_PLANE)
	{
		if (vector_push_back(&cx->obj_v, &prim_tmp) != SUCCESS)
			return (ERR_MALLOC);
		return (SUCCESS);
	}
	if (vector_push_back(&cx->plane_v, &prim_tmp) != SUCCESS)
		return (ERR_MALLOC);
	return (SUCCESS);
}


int	conv_prims_to_ctx(const t_scene_parsed *parsed, t_conv_ctx *cx)
{
	t_list				*node;
	const t_parsed_element	*elem;

	if (init_primitives(cx) != SUCCESS)
		return (ERR_MALLOC);
	node = parsed->objects;
	while (node)
	{
		elem = node->content;
		if (conv_push_elem_to_ctx(elem, cx) != SUCCESS)
			return (ERR_MALLOC);
		node = node->next;
	}
	return (SUCCESS);
}

