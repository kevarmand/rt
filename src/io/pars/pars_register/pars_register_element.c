#include "io.h"
#include "error_codes.h"
#include "type.h"


static int	push_copy_to_list(t_list **list_head, const t_parsed_element *element)
{
	t_parsed_element	*heap_copy;
	t_list				*new_node;

	heap_copy = (t_parsed_element *)malloc(sizeof(*heap_copy));
	if (heap_copy == NULL)
		return (ERR_ALLOC);
	*heap_copy = *element;
	new_node = ft_lstnew(heap_copy);
	if (new_node == NULL)
	{
		free(heap_copy);
		return (ERR_ALLOC);
	}
	ft_lstadd_back(list_head, new_node);
	return (SUCCESS);
}

static int	is_object_type(t_element_type type)
{
	return (type == ELEM_SPHERE
		|| type == ELEM_PLANE
		|| type == ELEM_CYLINDER
		|| type == ELEM_TRIANGLE);
}

int	pars_register_element(t_scene_parsed *scene,
			const t_parsed_element *element,
			t_elem_role role)
{
	if (is_object_type(element->type))
	{
		scene->presence_mask |= PRESENCE_OBJECT;
		return (push_copy_to_list(&scene->objects, element));
	}
	if (element->type == ELEM_CAMERA)
	{
		if (role == ELEM_ROLE_PRIMARY
			&& (scene->presence_mask & PRESENCE_CAM))
			return (ERR_PARS);
		if (role == ELEM_ROLE_PRIMARY)
			scene->presence_mask |= PRESENCE_CAM;
		return (push_copy_to_list(&scene->cameras, element));
	}
	if (element->type == ELEM_LIGHT)
	{
		if (role == ELEM_ROLE_PRIMARY
			&& (scene->presence_mask & PRESENCE_LIGHT))
			return (ERR_PARS);
		if (role == ELEM_ROLE_PRIMARY)
			scene->presence_mask |= PRESENCE_LIGHT;
		return (push_copy_to_list(&scene->lights, element));
	}
	return (ERR_PARS);
}
