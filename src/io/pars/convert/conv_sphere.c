#include "io.h"
#include "error_codes.h"
#include <stdlib.h>
#include "scene.h"

t_vec3f	vec3f_sub(t_vec3f a, t_vec3f b);

int copy_obj_sphere(const t_parsed_element *src, t_sphere *dst)
{

	dst->center = ((t_vec3f){
		src->data.sphere.center[0],
		src->data.sphere.center[1],
		src->data.sphere.center[2]});
	dst->radius = (src->data.sphere.diameter / 2.0f);
	return (SUCCESS);
}
