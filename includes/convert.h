#ifndef CONVERT_H
# define CONVERT_H

#include "libft.h"

typedef struct s_conv_ctx
{
	int		width;
	int		height;
	float	aspect;

	int		primitive_count;
	int		object_count;
	int		plane_count;
	int		light_count;
	int		camera_count;
	int		material_count;
	int		texture_count;

	//on va stocker les hasmap/vecteur ici : on fera une conversion propre a la fin

	//texture
	t_vector	tex_v;\
	t_hashmap	tex_m;

	//material
	t_vector	mat_v;
	t_hashmap	mat_m;

	//surface
	t_vector	surf_v;	

	//camera
	t_vector	cam_v;

	//light
	t_vector	light_v;

	//plane
	t_vector	plane_v;

	//object
	t_vector	obj_v;




	int		prim_w;
	int		obj_w;
	int		plane_w;
	int		light_w;
	int		cam_w;
	int		mat_w;
	int		tex_w;
}	t_conv_ctx;

#endif