/* src/io/test_parsing.c */
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "io.h"
#include "libft.h"
#include "errors.h"

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
	print_vec3("position", e->u_data.camera.position);
	print_vec3("orientation", e->u_data.camera.orientation);
	printf("  fov=%d\n", e->u_data.camera.fov);
}

static void print_light_elem(const t_parsed_element *e, int index)
{
	printf("- light[%d]\n", index);
	print_vec3("position", e->u_data.light.position);
	printf("  brightness=%.3f\n", e->u_data.light.brightness);
	print_rgb("rgb", e->u_data.light.rgb);
}

static void print_object_elem(const t_parsed_element *e, int index)
{
	printf("- object[%d] kind=%s\n", index, kind_to_str(e->type));
	if (e->type == ELEM_SPHERE)
	{
		print_vec3("center", e->u_data.sphere.center);
		printf("  diameter=%.6f\n", e->u_data.sphere.diameter);
		print_rgb("rgb", e->rgb);
	}
	else if (e->type == ELEM_PLANE)
	{
		print_vec3("origin", e->u_data.plane.origin);
		print_vec3("normal", e->u_data.plane.normal);
		print_rgb("rgb", e->rgb);
	}
	else if (e->type == ELEM_CYLINDER)
	{
		print_vec3("origin", e->u_data.cylinder.origin);
		print_vec3("axis", e->u_data.cylinder.axis);
		printf("  diameter=%.6f height=%.6f\n",
			e->u_data.cylinder.diameter, e->u_data.cylinder.height);
		print_rgb("rgb", e->rgb);
	}
	else if (e->type == ELEM_TRIANGLE)
	{
		print_vec3("A", e->u_data.triangle.vertex1);
		print_vec3("B", e->u_data.triangle.vertex2);
		print_vec3("C", e->u_data.triangle.vertex3);
		print_rgb("rgb", e->rgb);
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
static void	print_options_numbers(const t_element_options *o)
{
	printf("  options:\n");
	printf("    ior=%.6f reflection=%.6f refraction=%.6f shininess=%.6f\n",
		o->ior, o->reflection, o->refraction, o->shininess);
	printf("    diffuse=%.6f specular=%.6f ambient=%.6f\n",
		o->diffuse_weight, o->specular_weight, o->ambient_occlusion);
	printf("    uv_mod=%d uv=[%.6f %.6f %.6f %.6f %.6f %.6f]\n",
		o->uv_mod, o->uv[0], o->uv[1], o->uv[2], o->uv[3], o->uv[4], o->uv[5]);
}

static void	print_options_paths(const t_element_options *o)
{
	if (o->texture_id != -1)
		printf("    texture_id=%d\n", o->texture_id);
	if (o->bumpmap_id != -1)
		printf("    bumpmap_id=%d\n", o->bumpmap_id);
}

static void	print_options(const t_element_options *o)
{
	print_options_numbers(o);
	print_options_paths(o);
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
		print_options(&element->options);
		current_node = current_node->next;
		element_index += 1;
	}
}


void print_scene2(const t_scene_parsed *scene)
{
	printf("=== parsed scene ===\n");
	print_globals(&scene->globals);
	print_cameras(scene->cameras);
	print_lights(scene->lights);
	print_objects(scene->objects);
	printf("====================\n");
}

