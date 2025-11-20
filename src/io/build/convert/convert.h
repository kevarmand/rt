#ifndef CONVERT_H
# define CONVERT_H

#include "libft.h"

typedef int t_index;

typedef struct s_opt_ids
{
	t_index	albedo;
	t_index	normal;
}	t_opt_ids;

typedef struct s_conv_ctx
{
	int		primitive_count;
	int		object_count;
	int		plane_count;
	int		light_count;
	int		camera_count;
	int		material_count;
	int		texture_count;

	//texture
	t_vector	tex_v;
	t_hashmap	*tex_m;

	//material
	t_vector	mat_v;
	t_hashmap	*mat_m;

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

}	t_conv_ctx;

typedef struct s_scene_parsed t_scene_parsed;
typedef struct s_scene t_scene;


/* ************************************************************************ */
/*                                CONVERT                                   */
/* ************************************************************************ */

/***
 * Convert global settings from parsed to scene
 * @param parsed The parsed scene data
 * @param scene The scene to populate
 */
void	conv_globals(const t_scene_parsed *parsed, t_scene *scene);

/***
 * Count all elements in the parsed scene and store in conversion context
 * @param parsed The parsed scene data
 * @param cx The conversion context to populate
 */
void	conv_count_all(const t_scene_parsed *parsed, t_conv_ctx *cx);

/***
 * Build the conversion context from the parsed scene
 * @param parsed The parsed scene data
 * @param ctx The conversion context to build
 * @return SUCCESS on success, error code on failure
 */
int	conv_build_ctx(t_scene_parsed *parsed, t_conv_ctx *ctx);

/***
 * Build the final scene from the conversion context
 * @param cx The conversion context
 * @param scene The scene to populate
 * @return SUCCESS on success, error code on failure
 * @note Preconditions: cx is fully populated
 * and scene is empty
 */
int	conv_build_scene(t_conv_ctx *cx, t_scene *scene);


/* ************************************************************************** */
/*                         CONVERT - ELEMENTS                                 */
/* ************************************************************************** */

/***
 * Convert objects from parsed scene to conversion context
 * @param parsed The parsed scene data
 * @param cx The conversion context to populate
 * @return SUCCESS on success, error code on failure
 */
int conv_cameras(t_scene_parsed *parsed, t_conv_ctx *cx);

/* ************************************************************************** */
/*                        CONVERT - CTX LIFETIME                              */
/* ************************************************************************** */

/***
 * Initialize the conversion context
 * @param cx The conversion context to initialize
 */
void	init_ctx(t_conv_ctx *cx);

/***
 * Free resources associated with the conversion context
 * @param cx The conversion context to free
 */
void	free_ctx(t_conv_ctx *cx);

#endif