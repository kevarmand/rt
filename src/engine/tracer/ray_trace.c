// #include "rt.h"
// #include "type.h"
// #include "tracer.h"


    

// int	ray_trace(t_data *data, const t_ray *ray, t_color *out)
// {
// 	t_hit	hit;

// 	if (ray->depth >= data->render.max_depth || ray->weight < data->render.min_weight)
// 	{
// 		shd_shade_background(data, ray, out);
// 		return (SUCCESS);
// 	}
// 	hit_reset(&hit);
// 	if (!geom_pick_nearest(data, ray, &hit))
// 	{
// 		shd_shade_background(data, ray, out);
// 		return (SUCCESS);
// 	}
// 	if (hit_build(data, ray, &hit) == ERROR)
// 	{
// 		shd_shade_background(data, ray, out);
// 		return (SUCCESS);
// 	}
// 	shd_shade_surface(data, ray, &hit, out);
// 	return (SUCCESS);
// }


// static void inline color_init(t_color *color)
// {
// 	color->r = 0;
// 	color->g = 0;
// 	color->b = 0;
// }

// void	shd_shade_surface(const t_data *data, const t_ray *ray,
// 					const t_hit *hit, t_color *out)
// {
// 	t_color	tmp;


// 	color_init(&tmp);
// 	shd_add_ambient_light(data, hit, &tmp);
// 	shd_add_diffuse_light(data, ray, hit, &tmp);
// 	shd_add_specular_light(data, ray, hit, &tmp);
// 	shd_add_reflection(data, ray, hit, &tmp);   /* bonus */
// 	shd_add_refraction(data, ray, hit, &tmp);   /* bonus */
// 	*out = tmp;
// }

// int	hit_build(const t_data *data, const t_ray *ray, t_hit *hit)
// {
// 	hit->position = point_add(ray->origin, vector_scale(ray->direction, hit->distance));
// 	if (geom_get_normal(data, hit) == ERROR)
// 		return (ERROR);
// 	if (vector_dot(ray->direction, hit->normal) > 0)
// 	{
// 		hit->inside = 1;
// 		hit->normal = vector_scale(hit->normal, -1);
// 	}
// 	else
// 		hit->inside = 0;
// 	return (SUCCESS);
// }