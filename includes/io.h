# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <unistd.h>
# include <string.h>
# include "type.h"

#ifndef IO_H
# define IO_H

# define BUFFER_SIZE 1024
#include "libft.h"

typedef enum e_pmask
{
	PRESENCE_CAM = 1u << 0,
	PRESENCE_AMBIENT = 1u << 1,
	PRESENCE_RESOLUTION = 1u << 2,
	PRESENCE_OBJECT = 1u << 3,
	PRESENCE_LIGHT = 1u << 4
}	t_pmask;

typedef struct s_scene_parsed
{
	t_list	*objects;
	t_list	*cameras;
	t_list	*lights;
	t_para	globals;
	t_pmask	presence_mask;
}	t_scene_parsed;

typedef struct s_elem_options
{
	float		ior;
	float		reflection;
	float		refraction;
	float		shininess;
	float		diffuse_weight;
	float		specular_weight;
	float		ambient_occlusion;
	float		alpha;
	//ect ...
}	t_element_options;

typedef enum e_element_type {
	ELEM_NONE, ELEM_SPHERE, ELEM_PLANE, ELEM_CYLINDER, ELEM_CAMERA, ELEM_LIGHT, ELEM_AMBIENT
}	t_element_type;

typedef struct s_parsed_sphere
{
	float			center[3];
	float			diameter;
	int				rgb[3];
}	t_parsed_sphere;

typedef struct s_parsed_plane
{
	float			origin[3];
	float			normal[3];
	int				rgb[3];
}	t_parsed_plane;

typedef struct s_parsed_cylinder
{
	float			origin[3];
	float			axis[3];
	float			diameter;
	float			height;
	int				rgb[3];
}	t_parsed_cylinder;

typedef struct s_parsed_triangle
{
	float			vertex1[3];
	float			vertex2[3];
	float			vertex3[3];
	int				rgb[3];
}	t_parsed_triangle;

typedef struct s_pars_camera
{
	float			position[3];
	float			orientation[3];
	int				fov;
}	t_parsed_camera;

typedef struct s_parsed_light
{
	float			position[3];
	float			brightness;
	int				rgb[3];
}	t_parsed_light;

typedef struct s_parsed_element
{
	t_element_type			type;
	t_element_options		options;
	union {
		t_parsed_sphere		sphere;
		t_parsed_plane		plane;
		t_parsed_cylinder	cylinder;
		t_parsed_triangle	triangle;
		t_parsed_camera		camera;
		t_parsed_light		light;
	}	data;
}	t_parsed_element;


/**
 * @brief Parse a scene from a file and populate the provided t_scene structure.
 * @param filename The path to the scene file to be parsed. (no_null)
 * @param scene A pointer to a t_scene structure to be populated 
 * with the parsed data.
 * @return Returns 0 on success, or a non-zero error code on failure.
 */
int	pars_scene(const char *filename, t_scene *scene);



#endif

