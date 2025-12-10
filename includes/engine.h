/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 18:08:04 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/10 03:12:13 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENGINE_H
# define ENGINE_H

#include "vector.h"
#include "scene.h"
#include "render.h"

typedef struct s_engine
{
	int				width;
	int				height;

	int				render_mode;
	int				samples_per_pixel;

	int				active_cam;
	int				shadow_cam;
	int				cam_count;

	t_render		render;
}	t_engine;

typedef struct s_ray {
	t_vec3f	origin;
	t_vec3f	dir;
	t_vec3f	invdir;
	t_vec3f	orig_div;
	int		sign[3];
}	t_ray;

typedef enum e_hit_kind
{
	HIT_NONE,
	HIT_PLANE,
	HIT_PRIMITIVE
}	t_hit_kind;

typedef struct s_hit {
	t_hit_kind	kind;
	float		t;
	t_vec3f		point;
	t_vec3f		normal;
	t_vec3f		view_dir;
	int			primitive_id;
	int			surface_id;
	int			material_id;
	float		u;
	float		v;
	t_vec3f		albedo;
}	t_hit;

typedef struct s_shading_ctx
{
	int		depth;
	float	current_ior;
	float	contribution;
}	t_shading_ctx;


typedef struct s_scene t_scene;
typedef struct s_primitive t_primitive;
typedef struct s_plane t_plane;
typedef struct s_sphere t_sphere;
typedef struct s_cylinder t_cylinder;
typedef struct s_triangle t_triangle;
typedef struct s_torus t_torus;

/* ************************************************************************** */
/*								INTERSECTION								  */
/* ************************************************************************** */

double equa_second(double a, double b, double c);

int scene_hit(const t_scene *scene, const t_ray *ray, t_hit *out_hit);

int	inter_plane(const t_plane *plane, const t_ray *ray, float *dist);

int	inter_sphere(const t_sphere *sphere, const t_ray *ray, float *dist);

int	inter_cylinder(const t_cylinder *cylinder, const t_ray *ray, float *dist);

int	inter_triangle(const t_triangle *triangle, const t_ray *ray, float *dist);

int	inter_torus(const t_torus *torus, const t_ray *ray, float *dist);
int	inter_primitive(const t_primitive *primitive,
			const t_ray *ray, float *out_distance);

int	scene_is_occluded(const t_scene *scene, const t_ray *ray, float max_dist);

/* ************************************************************************** */
/*  							Shading									  	  */
/* ************************************************************************** */


int	shading_ray(const t_scene *scene, const t_ray *ray,
	t_shading_ctx *ctx, t_vec3f *color_out);

int	shade_hit(const t_scene *scene, const t_hit *hit, t_shading_ctx *ctx, t_vec3f *color_out);

void	shade_direct_lights(const t_scene *scene, const t_hit *hit, t_vec3f *color);
void	hit_build_geometry(const t_scene *scene, const t_ray *ray, t_hit *hit);

void	shade_ambient(const t_scene *scene, const t_hit *hit, t_vec3f *color);

void	shade_reflection(const t_scene *scene,
			const t_hit *hit, t_shading_ctx *ctx, t_vec3f *color);

void	shade_refraction(const t_scene *scene,
			const t_hit *hit, t_shading_ctx *ctx, t_vec3f *color);

void	apply_surface_shading(const t_scene *sc, t_hit *hit);

/* ************************************************************************** */
/*							ENGINE       									  */
/* ************************************************************************** */

/***
 * Initialize the engine structure
 * return SUCCESS if ok, error code otherwise
 * @param engine pointer to the engine structure
 * @param scene pointer to the scene structure
 * @return 0 on SUCCESS, error code otherwise
 */
int		engine_init(t_engine *engine, t_scene *scene);

/***
 *calculate the rigight vectors for each camera in the scene
 * return SUCCESS if ok, error code otherwise
 * @param engine pointer to the engine structure
 * @return 0 on SUCCESS, error code otherwise
 */
void		init_cam_views(t_scene *scene);

/***
 * Start the rendering threads
 * return SUCCESS if ok, error code otherwise
 * @param engine pointer to the engine structure
 * @return 0 on SUCCESS, error code otherwise
 */
int		engine_start_threads(t_data *data);

#endif
