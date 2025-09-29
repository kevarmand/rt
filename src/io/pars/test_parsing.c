/* src/io/test_parsing.c */
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "io.h"
#include "libft.h"
#include "error_codes.h"

/* Préconditions :
   - argv[1] pointe vers un fichier .rt lisible
   - libft fournit t_list et ft_lstclear
   - pars_* remplissent scene->objects/cameras/lights avec t_parsed_element* */

static const char *kind_to_str(t_element_type type)
{
	if (type == ELEM_SPHERE) return "sphere";
	if (type == ELEM_PLANE) return "plane";
	if (type == ELEM_CYLINDER) return "cylinder";
	if (type == ELEM_TRIANGLE) return "triangle";
	if (type == ELEM_CAMERA) return "camera";
	if (type == ELEM_LIGHT) return "light";
	if (type == ELEM_AMBIENT) return "ambient";
	return "unknown";
}

static void print_vec3(const char *label, const float v[3])
{
	printf("  %s=(%.6f,%.6f,%.6f)\n", label, v[0], v[1], v[2]);
}

static void print_rgb(const char *label, const int rgb[3])
{
	printf("  %s=(%d,%d,%d)\n", label, rgb[0], rgb[1], rgb[2]);
}

static void print_globals(const t_param *p)
{
	printf("globals\n");
	printf("  resolution=%dx%d\n", p->res_width, p->res_height);
	printf("  ambient_brightness=%.3f\n", p->brightness);
	printf("  ambient_color=(%.3f,%.3f,%.3f)\n",
		p->color[0], p->color[1], p->color[2]);
}

static void print_camera_elem(const t_parsed_element *e, int index)
{
	printf("- camera[%d]\n", index);
	print_vec3("position", e->data.camera.position);
	print_vec3("orientation", e->data.camera.orientation);
	printf("  fov=%d\n", e->data.camera.fov);
}

static void print_light_elem(const t_parsed_element *e, int index)
{
	printf("- light[%d]\n", index);
	print_vec3("position", e->data.light.position);
	printf("  brightness=%.3f\n", e->data.light.brightness);
	print_rgb("rgb", e->data.light.rgb);
}

static void print_object_elem(const t_parsed_element *e, int index)
{
	printf("- object[%d] kind=%s\n", index, kind_to_str(e->type));
	if (e->type == ELEM_SPHERE)
	{
		print_vec3("center", e->data.sphere.center);
		printf("  diameter=%.6f\n", e->data.sphere.diameter);
		print_rgb("rgb", e->data.sphere.rgb);
	}
	else if (e->type == ELEM_PLANE)
	{
		print_vec3("origin", e->data.plane.origin);
		print_vec3("normal", e->data.plane.normal);
		print_rgb("rgb", e->data.plane.rgb);
	}
	else if (e->type == ELEM_CYLINDER)
	{
		print_vec3("origin", e->data.cylinder.origin);
		print_vec3("axis", e->data.cylinder.axis);
		printf("  diameter=%.6f height=%.6f\n",
			e->data.cylinder.diameter, e->data.cylinder.height);
		print_rgb("rgb", e->data.cylinder.rgb);
	}
	else if (e->type == ELEM_TRIANGLE)
	{
		print_vec3("A", e->data.triangle.vertex1);
		print_vec3("B", e->data.triangle.vertex2);
		print_vec3("C", e->data.triangle.vertex3);
		print_rgb("rgb", e->data.triangle.rgb);
	}
}

static int list_size(const t_list *head)
{
	int count;

	count = 0;
	while (head)
	{
		count += 1;
		head = head->next;
	}
	return (count);
}

static void print_cameras(const t_list *head)
{
	const t_list			*current_node;
	const t_parsed_element	*element;
	int						element_index;

	printf("cameras (%d)\n", list_size(head));
	current_node = head;
	element_index = 0;
	while (current_node)
	{
		element = (const t_parsed_element *)current_node->content;
		print_camera_elem(element, element_index);
		current_node = current_node->next;
		element_index += 1;
	}
}

static void print_lights(const t_list *head)
{
	const t_list			*current_node;
	const t_parsed_element	*element;
	int						element_index;

	printf("lights (%d)\n", list_size(head));
	current_node = head;
	element_index = 0;
	while (current_node)
	{
		element = (const t_parsed_element *)current_node->content;
		print_light_elem(element, element_index);
		current_node = current_node->next;
		element_index += 1;
	}
}

static void print_objects(const t_list *head)
{
	const t_list			*current_node;
	const t_parsed_element	*element;
	int						element_index;

	printf("objects (%d)\n", list_size(head));
	current_node = head;
	element_index = 0;
	while (current_node)
	{
		element = (const t_parsed_element *)current_node->content;
		print_object_elem(element, element_index);
		current_node = current_node->next;
		element_index += 1;
	}
}

static void *return_ptr(void *p)
{
	return (p);
}

static void free_parsed_element(void *payload)
{
	free(return_ptr(payload));
}

static void clear_scene_lists(t_scene_parsed *scene)
{
	ft_lstclear(&scene->objects, free_parsed_element);
	ft_lstclear(&scene->cameras, free_parsed_element);
	ft_lstclear(&scene->lights, free_parsed_element);
}

static void print_scene(const t_scene_parsed *scene)
{
	printf("=== parsed scene ===\n");
	print_globals(&scene->globals);
	print_cameras(scene->cameras);
	print_lights(scene->lights);
	print_objects(scene->objects);
	printf("====================\n");
}

int main(int argc, char **argv)
{
	t_scene_parsed	scene_builder;
	int				file_desc;
	int				status;
	(void)argc;

	pars_init_scene(&scene_builder);
	file_desc = open(argv[1], O_RDONLY);
	if (file_desc == -1)
		return (2);
	status = pars_scene_fd(file_desc, &scene_builder);
	close(file_desc);
	if (status != SUCCESS)
		return (status);
	print_scene(&scene_builder);
	clear_scene_lists(&scene_builder);
	return (0);
}
