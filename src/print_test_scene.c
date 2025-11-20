#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io.h"
#include "libft.h"
#include "errors.h"
#include "scene.h"


/***
 * le but est de faire un petit programme de test pour verifier que le parsing et le chargement de scene fonctionne correctement
 * il affiche le contenue de la scene parsee element par element
 * 

 */
void print_globals(const t_scene *scene)
{
	printf("globals\n");
	printf("  resolution=%dx%d\n", scene->resolution_width, scene->resolution_height);
	printf("  ambient_color=(%.3f,%.3f,%.3f)\n",
		scene->ambient_color.x, scene->ambient_color.y, scene->ambient_color.z);
}



void print_cameras(const t_scene *scene)
{
	int i;

	printf("cameras (%d)\n", scene->camera_count);
	for (i = 0; i < scene->camera_count; i++)
	{
		const t_camera *cam = &scene->cameras[i];
		printf("- camera[%d]\n", i);
		printf("  position=(%.6f,%.6f,%.6f)\n", cam->origin.x, cam->origin.y, cam->origin.z);
		printf("  orientation=(%.6f,%.6f,%.6f)\n", cam->forward.x, cam->forward.y, cam->forward.z);
		printf("  fov=%d\n", cam->fov_deg);
	}
}

void print_lights(const t_scene *scene)
{
	int i;

	printf("lights (%d)\n", scene->light_count);
	for (i = 0; i < scene->light_count; i++)
	{
		const t_light *light = &scene->lights[i];
		printf("- light[%d]\n", i);
		printf("  position=(%.6f,%.6f,%.6f)\n", light->position.x, light->position.y, light->position.z);
		printf("  color=(%.3f,%.3f,%.3f)\n", light->color.x, light->color.y, light->color.z);
	}
}

void print_objects(const t_scene *scene)
{
	int i;

	printf("objects (%d)\n", scene->object_count);
	for (i = 0; i < scene->object_count; i++)
	{
		const t_primitive *obj = &scene->primitives[i];
		printf("- object[%d]\n", i);
		printf("  primitive_type=%d\n", obj->type);
	}
}

void print_plane(const t_scene *scene)
{
	int i;

	printf("planes (%d)\n", scene->plane_count);
	for (i = 0; i < scene->plane_count; i++)
	{
		const t_primitive *plane = &scene->planes[i];
		printf("- plane[%d]\n", i);
		printf("  primitive_type=%d\n", plane->type);
	}
} 


void print_scene(const t_scene *scene)
{
	printf("=== parsed scene ===\n");
	print_globals(scene);
	print_cameras(scene);
	print_lights(scene);
	print_objects(scene);
	print_plane(scene);
	printf("====================\n");
}