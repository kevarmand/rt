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
	printf("  resolution=%dx%d\n", scene->resolution_width, scene->resolution_height);;
	printf("  ambient_color=(%.3f,%.3f,%.3f)\n",
		scene->ambient_color[0],
		scene->ambient_color[1],
		scene->ambient_color[2]);
}



void print_cameras(const t_scene *scene)
{
	int i;

	printf("cameras (%d)\n", scene->camera_count);
	for (i = 0; i < scene->camera_count; i++)
	{
	 	const t_camera *cam = &scene->cameras[i];
	 	printf("- camera[%d]\n", i);
		(void)cam;
		printf("  position=(%.6f,%.6f,%.6f)\n", cam->origin.x, cam->origin.y, cam->origin.z);
		printf("  orientation=(%.6f,%.6f,%.6f)\n", cam->forward.x, cam->forward.y, cam->forward.z);
		printf("  fov=%1f\n", cam->fov_deg);
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
		printf("  position=(%.6f,%.6f,%.6f)\n", light->position[0], light->position[1], light->position[2]);
		printf("  color=(%.3f,%.3f,%.3f)\n", light->color[0], light->color[1], light->color[2]);
	}
}
//on va afficher un peu plus de donnee
static void	print_vec3(const char *label, t_vec3f v)
{
	printf("  %s=(%.6f,%.6f,%.6f)\n", label, v.x, v.y, v.z);
}

static void	print_prim_sphere(const t_sphere *sp)
{
	printf("  kind=sphere\n");
	print_vec3("center", sp->center);
	printf("  radius=%.6f diameter=%.6f\n",
		sp->radius, sp->radius * 2.0f);
}

static void	print_prim_triangle(const t_triangle *tr)
{
	t_vec3f	v1;
	t_vec3f	v2;

	v1 = vec3f_add(tr->v0, tr->edge1);
	v2 = vec3f_add(tr->v0, tr->edge2);
	printf("  kind=triangle\n");
	print_vec3("v0", tr->v0);
	print_vec3("v1", v1);
	print_vec3("v2", v2);
}

static void	print_prim_cylinder(const t_cylinder *cy)
{
	printf("  kind=cylinder\n");
	print_vec3("base", cy->base);
	print_vec3("axis", cy->axis);
	printf("  height=%.6f radius=%.6f\n",
		cy->height, cy->radius);
}

static void	print_prim_torus(const t_torus *to)
{
	printf("  kind=torus\n");
	print_vec3("center", to->center);
	printf("  R=%.6f r=%.6f\n", to->r0, to->r1);
}

static void	print_prim_plane(const t_plane *pl)
{
	printf("  kind=plane\n");
	print_vec3("normal", pl->normal);
	printf("  d=%.6f\n", pl->d);
}

static void	print_primitive_details(const t_primitive *primitive)
{
	if (primitive->type == PRIM_SPHERE)
		print_prim_sphere(&primitive->sp);
	else if (primitive->type == PRIM_TRIANGLE)
		print_prim_triangle(&primitive->tr);
	else if (primitive->type == PRIM_CYLINDER)
		print_prim_cylinder(&primitive->cy);
	else if (primitive->type == PRIM_TORUS)
		print_prim_torus(&primitive->to);
	else if (primitive->type == PRIM_PLANE)
		print_prim_plane(&primitive->pl);
}

void	print_objects(const t_scene *scene)
{
	int				index;
	const t_primitive	*primitive;

	printf("objects (%d)\n", scene->primitive_count);
	index = 0;
	while (index < scene->primitive_count)
	{
		primitive = &scene->primitives[index];
		printf("- object[%d]\n", index);
		printf("  primitive_type=%d\n", primitive->type);
		printf("  material_id=%d surface_id=%d\n",
			primitive->material_id, primitive->surface_id);
		print_primitive_details(primitive);
		index++;
	}
}


void print_plane(const t_scene *scene)
{
	int i;

	printf("planes (%d)\n", scene->plane_count);
	for (i = 0; i < scene->plane_count; i++)
	{
		const t_primitive *plane = &scene->planes[i];
			printf("  material_id=%d surface_id=%d\n",
			plane->material_id, plane->surface_id);
		printf("- plane[%d]\n", i);
		printf("  primitive_type=%d\n", plane->type);
	}
}

void print_surfaces(const t_scene *scene)
{
	int i;

	printf("surfaces (%d)\n", scene->surface_count);
	for (i = 0; i < scene->surface_count; i++)
	{
		const t_surface_map *surface = &scene->surfaces[i];
		printf("- surface[%d]\n", i);
		print_vec3("color", surface->color);
		printf("  texture_albedo_id=%d bump_normal_id=%d\n",
			surface->texture_albedo_id, surface->bump_normal_id);
		printf("  checker_mode=%d checker_texture_id=%d\n",
			surface->checker_mode, surface->checker_texture_id);
		print_vec3("checker_color", surface->checker_color);
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
	print_surfaces(scene);
	printf("number of surfaces: %d\n", scene->surface_count);
	printf("number of materials: %d\n", scene->material_count);
	printf("number of textures: %d\n", scene->texture_count);

	printf("====================\n");
}
