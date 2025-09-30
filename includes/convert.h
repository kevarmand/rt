#ifndef CONVERT_H
# define CONVERT_H


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

	int		prim_w;
	int		obj_w;
	int		plane_w;
	int		light_w;
	int		cam_w;
	int		mat_w;
	int		tex_w;
}	t_conv_ctx;

#endif