#include "io.h"


static void free_element(void *elem)
{
	//on check les chaine dans oiption
	t_parsed_element	*e;
	e = (t_parsed_element *)elem;
	if (!elem)
		return ;
	if (e->options.texture_path)
		free(e->options.texture_path);
	if (e->options.bumpmap_path)
		free(e->options.bumpmap_path);
	free(elem);
}

static void cleanup_tlist(t_list **lst, void (*del)(void *))
{
	t_list	*current;
	t_list	*next;

	current = *lst;
	while (current)
	{
		next = current->next;
		if (del)
			del(current->content);
		free(current);
		current = next;
	}
	*lst = NULL;
}

void free_parsed_scene(t_scene_parsed *scene)
{
	cleanup_tlist(&scene->cameras, free_element);
	cleanup_tlist(&scene->lights, free_element);
	cleanup_tlist(&scene->objects, free_element);
}
