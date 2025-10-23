#include "rt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libft.h"
#include "io.h"
#include "scene.h"


int	scene_load_pipeline(const char *filename, t_scene *scene);

/***
 * delete the scene and free all allocated memory
 */
void	free_scene(t_scene *scene)
{
	if (!scene)
		return ;
	if (scene->primitives)
		free(scene->primitives);
	if (scene->objects)
		free(scene->objects);
	if (scene->bvh_nodes)
		free(scene->bvh_nodes);
	if (scene->planes)
		free(scene->planes);
	if (scene->materials)
		free(scene->materials);
	if (scene->surfaces)
		free(scene->surfaces);
	if (scene->textures)
		free(scene->textures);
	if (scene->lights)
		free(scene->lights);
	if (scene->cameras)
		free(scene->cameras);
}

/***
 * petit main de test (pour linstant il est dnas le parsing mais on garde une facon clair pour la reste)
 */
int	main(int argc, char **argv)
{
	t_scene	scene;
	int		status;

	ft_memset(&scene, 0, sizeof(t_scene));
	status = scene_load_pipeline(argv[1], &scene);
	if (status != SUCCESS)
	{
		print_error("Error loading scene", status);
		return (status);
	}
	printf("Scene loaded successfully:\n");
	//afficher quelques infos sur la scene
	printf("Resolution: %dx%d\n", scene.resolution_width, scene.resolution_height);
	printf("Number of objects: %d\n", scene.object_count);
	printf("Number of lights: %d\n", scene.light_count);
	// Nettoyage
	free_scene(&scene);
	return (0);
}