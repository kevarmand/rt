#include "engine.h"
#include "scene.h"
#include <math.h>
#include "attributes.h"

t_vec3f sample_texture(const t_texture *tex, float u, float v);

/***helper globaux**/
static float	clamp_unit(float value)
{
	if (value > 1.0f)
		return (1.0f);
	if (value < -1.0f)
		return (-1.0f);
	return (value);
}

FORCEINLINE
void	cylinder_build_frame(t_vec3f axis, t_vec3f *t0, t_vec3f *b0)
{
	t_vec3f	ref_axis;

	ref_axis = (t_vec3f){0.0f, 1.0f, 0.0f};
	if (fabsf(axis.y) > 0.999f)
		ref_axis = (t_vec3f){1.0f, 0.0f, 0.0f};
	*t0 = vec3f_cross(ref_axis, axis);
	*t0 = vec3f_normalize(*t0);
	*b0 = vec3f_cross(axis, *t0);
	*b0 = vec3f_normalize(*b0);
}

/*****Helper normal*******/
FORCEINLINE
void	hit_faceforward(const t_ray *ray, t_hit *hit)
{
	if (vec3f_dot(hit->normal, ray->dir) > 0.0f)
		hit->normal = vec3f_scale(hit->normal, -1.0f);
}


FORCEINLINE 
void	sphere_build_normal(const t_sphere *sphere, t_hit *hit)
{
	t_vec3f	local;

	local = vec3f_sub(hit->point, sphere->center);
	hit->normal = vec3f_normalize(local);
}

FORCEINLINE
void	cylinder_build_normal(const t_cylinder *cylinder, t_hit *hit)
{
	t_vec3f	base_to_point;
	float	height_pos;
	t_vec3f	proj_point;
	t_vec3f	radial;

	base_to_point = vec3f_sub(hit->point, cylinder->base);
	height_pos = vec3f_dot(base_to_point, cylinder->axis);
	proj_point = vec3f_add(cylinder->base,
			vec3f_scale(cylinder->axis, height_pos));
	radial = vec3f_sub(hit->point, proj_point);
	hit->normal = vec3f_normalize(radial);
}


/*****Helper maping uv***/ 
void	surface_apply_uv(const t_surface_map *surface, t_hit *hit)
{
	float	base_u;
	float	base_v;

	base_u = hit->u;
	base_v = hit->v;
	hit->u = surface->map_uv[0] * base_u
		+ surface->map_uv[1] * base_v
		+ surface->map_uv[4];
	hit->v = surface->map_uv[2] * base_u
		+ surface->map_uv[3] * base_v
		+ surface->map_uv[5];
}

FORCEINLINE
void	sphere_build_uv(const t_sphere *sphere, t_hit *hit)
{
	t_vec3f	local;
	float	phi;
	float	theta;

	local = vec3f_sub(hit->point, sphere->center);
	local = vec3f_normalize(local);
	phi = atan2f(local[2], local[0]);
	if (phi < 0.0f)
		phi += 2.0f * (float)M_PI;
	theta = acosf(clamp_unit(local[1]));
	hit->u = phi / (2.0f * (float)M_PI);
	hit->v = theta / (float)M_PI;
}

FORCEINLINE
void	cylinder_build_uv(const t_cylinder *cylinder, t_hit *hit)
{
	t_vec3f	base_to_point;
	float	height_pos;
	t_vec3f	proj_point;
	t_vec3f	normal_raw;
	t_vec3f	t0;
	t_vec3f	b0;
	float	x;
	float	y;
	float	phi;

	base_to_point = vec3f_sub(hit->point, cylinder->base);
	height_pos = vec3f_dot(base_to_point, cylinder->axis);
	proj_point = vec3f_add(cylinder->base, vec3f_scale(cylinder->axis, height_pos));
	normal_raw = vec3f_normalize(vec3f_sub(hit->point, proj_point));
	cylinder_build_frame(cylinder->axis, &t0, &b0);
	x = vec3f_dot(normal_raw, t0);
	y = vec3f_dot(normal_raw, b0);
	phi = atan2f(y, x);
	if (phi < 0.0f)
		phi += 2.0f * (float)M_PI;
	hit->u = phi / (2.0f * (float)M_PI);
	hit->v = height_pos / cylinder->height;
}

FORCEINLINE
void	triangle_build_uv_bary(const t_surface_map *surface,
			float bary_u, float bary_v, t_hit *hit)
{
	float	bary_w;

	bary_w = 1.0f - bary_u - bary_v;
	hit->u = surface->map_uv[0] * bary_w
		+ surface->map_uv[2] * bary_u
		+ surface->map_uv[4] * bary_v;
	hit->v = surface->map_uv[1] * bary_w
		+ surface->map_uv[3] * bary_u
		+ surface->map_uv[5] * bary_v;
}

FORCEINLINE
void	plane_build_uv(const t_surface_map *surface, t_hit *hit)
{
	hit->u = vec3f_dot(hit->point, surface->tang);
	hit->v = vec3f_dot(hit->point, surface->bitang);
	surface_apply_uv(surface, hit);
}

/**Helper BT**/

FORCEINLINE
void	surface_grad_to_base(const t_surface_map *surface,
			t_vec3f grad_final, t_vec3f *grad_base)
{
	grad_base->x = surface->uv_jacobian_t[0] * grad_final.x
		+ surface->uv_jacobian_t[1] * grad_final.y;
	grad_base->y = surface->uv_jacobian_t[2] * grad_final.x
		+ surface->uv_jacobian_t[3] * grad_final.y;
	grad_base->z = 0.0f;
}

FORCEINLINE
void	sphere_build_tbn(t_vec3f normal_raw, t_vec3f *tangent, t_vec3f *bitangent)
{
	t_vec3f	tan;

	tan = (t_vec3f){-normal_raw.z, 0.0f, normal_raw.x};
	if (fabsf(normal_raw.y) > 0.999f)
	{
		tan = (t_vec3f){1.0f, 0.0f, 0.0f};
		if (fabsf(normal_raw.x) > 0.999f)
			tan = (t_vec3f){0.0f, 0.0f, 1.0f};
	}
	*tangent = vec3f_normalize(tan);
	*bitangent = vec3f_cross(*tangent, normal_raw);
	*bitangent = vec3f_normalize(*bitangent);
}

FORCEINLINE
void	cylinder_build_tbn(const t_cylinder *cylinder, t_vec3f normal_raw,
			t_vec3f *tangent, t_vec3f *bitangent)
{
	*bitangent = cylinder->axis;
	*tangent = vec3f_cross(*bitangent, normal_raw);
	*tangent = vec3f_normalize(*tangent);
	*bitangent = vec3f_normalize(*bitangent);
}



/******build element***/
void	build_geo_sphere(const t_scene *scene,
			const t_ray *ray, const t_sphere *sphere, t_hit *hit)
{
	const t_surface_map	*surface;
	int					needs_uv;
	int					needs_bump;
	t_vec3f				tangent;
	t_vec3f				bitangent;
	t_vec3f				grad_final;
	t_vec3f				grad_base;
	t_vec3f				normal_raw;

	surface = &scene->surfaces[hit->surface_id];
	needs_bump = (surface->bump_normal_id != SCENE_ID_NONE);
	needs_uv = (needs_bump || surface->checker_mode != 0
			|| surface->texture_albedo_id != SCENE_ID_NONE);
	sphere_build_normal(sphere, hit);
	normal_raw = hit->normal;
	if (needs_uv)
	{
		sphere_build_uv(sphere, hit);
		surface_apply_uv(surface, hit);
	}
	if (needs_bump)
	{

		sphere_build_tbn(normal_raw, &tangent, &bitangent);
		grad_final = sample_texture(&scene->bumpmaps[surface->bump_normal_id],
				hit->u, hit->v);
		surface_grad_to_base(surface, grad_final, &grad_base);
		normal_raw = vec3f_add(normal_raw, vec3f_scale(tangent,
					grad_base.x * surface->bump_scale));
		normal_raw = vec3f_add(normal_raw, vec3f_scale(bitangent,
					grad_base.y * surface->bump_scale));
		normal_raw = vec3f_normalize(normal_raw);
	}
	hit->normal = normal_raw;
	hit_faceforward(ray, hit);
}

void	build_geo_cylinder(const t_scene *scene,
			const t_ray *ray, const t_cylinder *cylinder, t_hit *hit)
{
	const t_surface_map	*surface;
	int					needs_uv;
	int					needs_bump;
	t_vec3f				tangent;
	t_vec3f				bitangent;
	t_vec3f				grad_final;
	t_vec3f				grad_base;
	t_vec3f				normal_raw;

	surface = &scene->surfaces[hit->surface_id];
	needs_bump = (surface->bump_normal_id != SCENE_ID_NONE);
	needs_uv = (needs_bump || surface->checker_mode != 0
			|| surface->texture_albedo_id != SCENE_ID_NONE);
	cylinder_build_normal(cylinder, hit);
	normal_raw = hit->normal;
	if (needs_uv)
	{
		cylinder_build_uv(cylinder, hit);
		surface_apply_uv(surface, hit);
	}
	if (needs_bump)
	{
		cylinder_build_tbn(cylinder, normal_raw, &tangent, &bitangent);
		grad_final = sample_texture(&scene->bumpmaps[surface->bump_normal_id],
				hit->u, hit->v);
		surface_grad_to_base(surface, grad_final, &grad_base);
		normal_raw = vec3f_add(normal_raw, vec3f_scale(tangent,
					grad_base.x * surface->bump_scale));
		normal_raw = vec3f_add(normal_raw, vec3f_scale(bitangent,
					grad_base.y * surface->bump_scale));
		normal_raw = vec3f_normalize(normal_raw);
	}
	hit->normal = normal_raw;
	hit_faceforward(ray, hit);
}


void	build_geo_triangle(const t_scene *scene,
			const t_ray *ray, const t_surface_map *surface, t_hit *hit)
{
	int		needs_uv;
	int		needs_bump;
	float	bary_u;
	float	bary_v;
	t_vec3f	normal_raw;
	t_vec3f	grad_final;
	t_vec3f	grad_base;

	needs_bump = (surface->bump_normal_id != SCENE_ID_NONE);
	needs_uv = (needs_bump || surface->checker_mode != 0
			|| surface->texture_albedo_id != SCENE_ID_NONE);
	bary_u = hit->u;
	bary_v = hit->v;
	normal_raw = surface->normal;
	if (needs_uv)
		triangle_build_uv_bary(surface, bary_u, bary_v, hit);
	if (needs_bump)
	{
		grad_final = sample_texture(&scene->bumpmaps[surface->bump_normal_id],
				hit->u, hit->v);
		surface_grad_to_base(surface, grad_final, &grad_base);
		normal_raw = vec3f_add(normal_raw, vec3f_scale(surface->tang,
					grad_base.x * surface->bump_scale));
		normal_raw = vec3f_add(normal_raw, vec3f_scale(surface->bitang,
					grad_base.y * surface->bump_scale));
		normal_raw = vec3f_normalize(normal_raw);
	}
	hit->normal = normal_raw;
	hit_faceforward(ray, hit);
}

void	build_geo_plane(const t_scene *scene,
			const t_ray *ray, const t_plane *plane, t_hit *hit)
{
	const t_surface_map	*surface;
	int					needs_uv;
	int					needs_bump;
	t_vec3f				normal_raw;
	t_vec3f				grad_final;
	t_vec3f				grad_base;

	surface = &scene->surfaces[hit->surface_id];
	needs_bump = (surface->bump_normal_id != SCENE_ID_NONE);
	needs_uv = (needs_bump || surface->checker_mode != 0
			|| surface->texture_albedo_id != SCENE_ID_NONE);
	normal_raw = plane->normal;
	if (needs_uv)
		plane_build_uv(surface, hit);
	if (needs_bump)
	{
		grad_final = sample_texture(&scene->bumpmaps[surface->bump_normal_id],
				hit->u, hit->v);
		surface_grad_to_base(surface, grad_final, &grad_base);
		normal_raw = vec3f_add(normal_raw, vec3f_scale(surface->tang,
					grad_base.x * surface->bump_scale));
		normal_raw = vec3f_add(normal_raw, vec3f_scale(surface->bitang,
					grad_base.y * surface->bump_scale));
		normal_raw = vec3f_normalize(normal_raw);
	}
	hit->normal = normal_raw;
	hit_faceforward(ray, hit);
}


/* *** Main fonction ** * */


void	hit_build_geometry(const t_scene *scene, const t_ray *ray, t_hit *hit)
{
	const t_primitive	*primitive;
	const t_surface_map	*surface;

	hit->point = vec3f_add(ray->origin, vec3f_scale(ray->dir, hit->t));
	hit->view_dir = vec3f_scale(ray->dir, -1.0f);
	if (hit->kind == HIT_PLANE)
	{
		primitive = &scene->planes[hit->primitive_id];
		surface = &scene->surfaces[primitive->surface_id];
		hit->surface_id = primitive->surface_id;
		hit->material_id = primitive->material_id;
		build_geo_plane(scene, ray, &primitive->pl, hit);
	}
	else
	{
		primitive = &scene->primitives[hit->primitive_id];
		surface = &scene->surfaces[primitive->surface_id];
		hit->surface_id = primitive->surface_id;
		hit->material_id = primitive->material_id;
		if (primitive->type == PRIM_SPHERE)
			build_geo_sphere(scene, ray, &primitive->sp, hit);
		else if (primitive->type == PRIM_CYLINDER)
			build_geo_cylinder(scene, ray, &primitive->cy, hit);
		else if (primitive->type == PRIM_TRIANGLE)
			build_geo_triangle(scene, ray, surface, hit);
	}
}
