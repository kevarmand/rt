#include "rt.h"
#include "type.h"
#include "tracer.h"


    

int	ray_trace(t_data *data, const t_ray *ray, t_color *out)
{
    t_hit   hit;

    hit_init(&hit);
    if (find_nearest(data, ray, &hit))
    {
        hit_calc_normal(&hit);
        hit_calc_color(data, &hit, out);
    }
    else
    {
        no_object_hit(data->skybox, out);
    }
    return (SUCCESS);
}

static void inline color_init(t_color *color)
{
	color->r = 0;
	color->g = 0;
	color->b = 0;
}

void	shade_surface(const t_data *data, const t_ray *ray,
					const t_hit *hit, t_color *out)
{
	t_color	tmp;

    color_init(&tmp);
    //on fait une fonction par type d elumiere 
    hit_add_ambient_light(data, hit, &tmp);
    hit_add_diffuse_light(data, ray, hit, &tmp);
    hit_add_specular_light(data, ray, hit, &tmp);
    hit_add_reflection(data, ray, hit, &tmp);
    hit_add_refraction(data, ray, hit, &tmp);
	*out = tmp;
}
