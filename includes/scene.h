/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 22:07:39 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 01:07:29 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCENE_H
# define SCENE_H

# include "vector.h"
# include <stdint.h>

typedef int	t_index;

# define SCENE_ID_NONE -1

typedef struct s_aabb
{
	t_vec3f	b[2];
}	t_aabb;

typedef enum e_primtype
{
	PRIM_TRIANGLE,
	PRIM_PLANE,
	PRIM_SPHERE,
	PRIM_CYLINDER,
	PRIM_TORUS,
}	t_primtype;

typedef struct s_triangle
{
	t_vec3f	v0;
	t_vec3f	edge1;
	t_vec3f	edge2;
}	t_triangle;

typedef struct s_sphere
{
	t_vec3f	center;
	float	radius;
	float	r_squared;
}	t_sphere;

typedef struct s_cylinder
{
	t_vec3f	base;
	t_vec3f	axis;
	float	height;
	float	radius;
	float	r_squared;
}	t_cylinder;

typedef struct s_torus
{
	t_vec3f	center;
	t_vec3f	normal;
	float	r0;
	float	r1;
	float	r_squared;
	float	R_squared;
}	t_torus;

typedef struct s_plane
{
	t_vec3f	normal;
	float	d;
}	t_plane;

typedef struct s_primitive
{
	t_primtype	type;
	union {
		t_triangle	tr;
		t_sphere	sp;
		t_cylinder	cy;
		t_torus		to;
		t_plane		pl;
	};
	t_index		material_id;
	t_index		surface_id;
}	t_primitive;

typedef struct s_priminfo
{
	t_aabb	bounds;
	t_vec3f	centroid;
}	t_priminfo;

typedef struct s_object
{
	const char	*name;
	t_index		first_prim;
	int			prim_count;
	t_aabb		bounds;
}	t_object;

typedef struct s_texture
{
	char	*path;
	int		width;
	int		height;
	int		channels;
	void	*pixels;
}	t_texture;

typedef struct s_surface
{
	float	map_uv[6];
	int		uv_mod;
	float	uv_jacobian_t[4];
	t_vec3f	tang;
	t_vec3f	bitang;
	t_vec3f	color;
	t_vec3f	normal;
	t_index	texture_albedo_id;
	t_index	bump_normal_id;
	float	bump_scale;
	int		checker_mode;
	int		checker_texture_id;
	t_vec3f	checker_color;
}	t_surface_map;

typedef struct s_material
{
	float	ambient;
	float	diffuse;
	float	specular;
	float	shininess;
	float	reflection;
	float	refraction;
	float	ior;
}	t_material;

typedef struct s_light
{
	t_vec3f	position;
	t_vec3f	color;
	float	intensity;
}	t_light;

typedef struct s_camera
{
	t_vec3f	origin;
	t_vec3f	forward;
	t_vec3f	right;
	float	fov_deg;
}	t_camera;

/* --------- BVH (runtime) ---------
 * Nœuds immuables; feuilles pointent sur une plage contiguë de primitives.
 */
typedef struct s_bvhnode
{
	t_aabb	bounds;
	uint8_t	is_leaf;
	union
	{
		struct s_node
		{
			uint32_t	left;
			uint32_t	right;
		}	node;
		struct s_leaf
		{
			uint32_t	start;
			uint32_t	count;
		}	leaf;
	};
}	t_bvhnode;

typedef struct s_skybox
{
	t_index		texture_id;
	int			mode;
	t_vec3f		intensity;
}	t_skybox;

typedef struct s_scene
{
	int				resolution_width;
	int				resolution_height;
	t_vec3f			ambient_color;

	t_skybox		skybox;

	t_primitive		*primitives;
	int				primitive_count;

	t_bvhnode		*bvh_nodes;
	int				bvh_node_count;
	t_index			bvh_root_id;

	t_primitive		*planes;
	int				plane_count;

	t_material		*materials;
	int				material_count;

	t_surface_map	*surfaces;
	int				surface_count;

	t_texture		*textures;
	int				texture_count;

	t_texture		*bumpmaps;
	int				bumpmap_count;

	t_light			*lights;
	int				light_count;

	t_camera		*cameras;
	int				camera_count;

}	t_scene;

#endif
