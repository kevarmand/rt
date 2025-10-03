#include "io.h"
#include "error_codes.h"
#include <stdlib.h>
#include "scene.h"

int copy_obj_cylinder(const t_parsed_element *src, t_cylinder *dst)
{

	dst->p0 = (t_vec3f){
		src->data.cylinder.origin[0],
		src->data.cylinder.origin[1],
		src->data.cylinder.origin[2]};
	dst->p1 = (t_vec3f){
		src->data.cylinder.origin[0] + src->data.cylinder.axis[0]
			* src->data.cylinder.height,
		src->data.cylinder.origin[1] + src->data.cylinder.axis[1]
			* src->data.cylinder.height,
		src->data.cylinder.origin[2] + src->data.cylinder.axis[2]
			* src->data.cylinder.height};
	dst->radius = (src->data.cylinder.diameter / 2.0f);
	return (SUCCESS);
}
