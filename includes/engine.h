/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 18:08:04 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/23 15:13:26 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENGINE_H
# define ENGINE_H

#include "vector.h"
#include "scene.h"
#include "render.h"

typedef struct s_cam_view
{
	const t_camera	*src;
	t_vec3f			origin;
	t_vec3f			forward;
	t_vec3f			right;
	t_vec3f			up;
	t_vec3f			p0;
	t_vec3f			dx;
	t_vec3f			dy;

	t_vec3f			*hdr_buffer;
	int				hdr_width;
	int				hdr_height;
	int				is_dirty;
}	t_cam_view;


typedef struct s_engine
{
	int				width;
	int				height;

	int				render_mode;
	int				samples_per_pixel;

	int				active_cam;
	int				cam_count;
	t_cam_view		*cam_views;

	t_render		render;
}	t_engine;


typedef struct s_ray {
	t_vec3f	origin;
	t_vec3f	dir;
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
	int			primitive_id;
}	t_hit;

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

int equa_second(double a, double b, double c);

int scene_hit(const t_scene *scene, const t_ray *ray, t_hit *out_hit);

int	inter_plane(const t_plane *plane, const t_ray *ray, float *dist);

int	inter_sphere(const t_sphere *sphere, const t_ray *ray, float *dist);

int	inter_cylinder(const t_cylinder *cylinder, const t_ray *ray, float *dist);

int	inter_triangle(const t_triangle *triangle, const t_ray *ray, float *dist);

int	inter_torus(const t_torus *torus, const t_ray *ray, float *dist);

int	scene_is_occluded(const t_scene *scene, const t_ray *ray, float max_dist);

/* ************************************************************************** */
/*  							Shading									  	  */
/* ************************************************************************** */


int	shading_ray(const t_scene *scene, t_ray *ray, t_vec3f *color_out);

int	shade_hit(const t_scene *scene, t_hit *hit, t_vec3f *color_out);

void	shade_lights(const t_scene *scene, const t_hit *hit, t_vec3f *color);
void	hit_build_geometry(const t_scene *scene, const t_ray *ray, t_hit *hit);

void	shade_ambient(const t_scene *scene, const t_hit *hit, t_vec3f *color);

#endif