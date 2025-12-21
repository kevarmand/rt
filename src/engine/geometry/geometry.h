/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   geometry.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 19:07:56 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 06:45:01 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GEOMETRY_H
# define GEOMETRY_H

# include "engine.h"
# include "scene.h"
# include <math.h>

typedef struct s_bump_apply
{
	const t_scene		*scene;
	const t_surface_map	*surface;
	t_vec3f				tangent;
	t_vec3f				bitangent;
	float				u;
	float				v;
}	t_bump_apply;

int		surface_needs_uv(const t_surface_map *surface);
int		surface_needs_bump(const t_surface_map *surface);
void	bump_apply_normal(const t_bump_apply *bump, t_vec3f *normal);

void	hit_faceforward(const t_ray *ray, t_hit *hit);

void	surface_apply_uv(const t_surface_map *surface, t_hit *hit);
void	sphere_build_uv(const t_sphere *sphere, t_hit *hit);
void	cylinder_build_uv(const t_cylinder *cylinder, t_hit *hit);
void	triangle_build_uv_bary(const t_surface_map *surface,
			float bary_u, float bary_v, t_hit *hit);
void	plane_build_uv(const t_surface_map *surface, t_hit *hit);

void	sphere_build_tbn(t_vec3f normal_raw,
			t_vec3f *tangent, t_vec3f *bitangent);
void	cylinder_build_tbn(const t_cylinder *cylinder,
			t_vec3f normal_raw,
			t_vec3f *tangent, t_vec3f *bitangent);

void	build_geo_sphere(const t_scene *scene,
			const t_ray *ray,
			const t_sphere *sphere,
			t_hit *hit);
void	build_geo_cylinder(const t_scene *scene,
			const t_ray *ray,
			const t_cylinder *cylinder,
			t_hit *hit);
void	build_geo_plane(const t_scene *scene,
			const t_ray *ray,
			const t_plane *plane,
			t_hit *hit);
void	build_geo_triangle(const t_scene *scene,
			const t_ray *ray,
			const t_surface_map *surface,
			t_hit *hit);

t_vec3f	torus_compute_radial(const t_torus *torus, t_vec3f p);
void	torus_build_uv(const t_torus *torus, t_vec3f p, t_vec3f radial,
			t_hit *hit);
void	torus_set_tbn(const t_torus *torus, t_vec3f radial, t_vec3f normal,
			t_bump_apply *bump);
void	geo_build_frame(t_vec3f axis, t_vec3f *t0, t_vec3f *b0);

void	build_geo_torus(const t_scene *scene,
			const t_ray *ray,
			const t_torus *torus,
			t_hit *hit);

t_vec3f	sample_texture(const t_texture *tex, float u, float v);

#endif
