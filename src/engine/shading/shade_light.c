/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shade_light.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 15:04:24 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/22 16:50:14 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include "scene.h"

static void	shade_one_light(const t_scene *scene,
			const t_hit *hit, int light_index, t_vec3f *color)
{
	const t_primitive	*primitive;
	const t_material	*material;
	const t_surface		*surface;
	const t_light		*light;
	t_vec3f				light_vec;
	t_vec3f				light_dir;
	t_vec3f				offset_origin;
	t_ray				shadow_ray;
	float				light_dist;
	float				ndotl;
	t_vec3f				tmp;

	primitive = &scene->primitives[hit->primitive_id];
	material = &scene->materials[primitive->material_id];
	surface = &scene->surfaces[primitive->surface_id];
	light = &scene->lights[light_index];
	light_vec = vec3f_sub(light->position, hit->point);
	light_dist = vec3f_length(light_vec);
	if (light_dist <= 0.0f)
		return ;
	light_dir = vec3f_scale(light_vec, 1.0f / light_dist);
	offset_origin = vec3f_add(hit->point,
			vec3f_scale(hit->normal, 1e-4f));
	shadow_ray.origin = offset_origin;
	shadow_ray.dir = light_dir;
	if (scene_is_occluded(scene, &shadow_ray, light_dist))
		return ;
	ndotl = vec3f_dot(hit->normal, light_dir);
	if (ndotl <= 0.0f)
		return ;
	tmp = vec3f_mul(surface->color, light->color);
	tmp = vec3f_scale(tmp, material->diffuse
			* light->intensity * ndotl);
	*color = vec3f_add(*color, tmp);
}


void	shade_lights(const t_scene *scene,
			const t_hit *hit, t_vec3f *color)
{
	int	light_index;

	light_index = 0;
	while (light_index < scene->light_count)
	{
		shade_one_light(scene, hit, light_index, color);
		light_index++;
	}
}


