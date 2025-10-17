#include "io.h"
#include "error_codes.h"
#include <stdlib.h>
#include "scene.h"

t_vec3f	vec3f_sub(t_vec3f a, t_vec3f b);


int	copy_triangle_to_primitive(const t_parsed_element *src, t_triangle *dst)
{

	dst->v0 = ((t_vec3f){
		src->data.triangle.vertex1[0],
		src->data.triangle.vertex1[1],
		src->data.triangle.vertex1[2]});
	dst->edge1 = vec3f_sub((t_vec3f){
		src->data.triangle.vertex2[0],
		src->data.triangle.vertex2[1],
		src->data.triangle.vertex2[2]},
		dst->v0);
	dst->edge2 = vec3f_sub((t_vec3f){
		src->data.triangle.vertex3[0],
		src->data.triangle.vertex3[1],
		src->data.triangle.vertex3[2]},
		dst->v0);
	return (SUCCESS);
}
int	copy_sphere_to_primitive(const t_parsed_element *src, t_sphere *dst)
{

	dst->center = ((t_vec3f){
		src->data.sphere.center[0],
		src->data.sphere.center[1],
		src->data.sphere.center[2]});
	dst->radius = (src->data.sphere.diameter / 2.0f);
	return (SUCCESS);
}

int	copy_cylinder_to_primitive(const t_parsed_element *src, t_cylinder *dst)
{

	dst->p0 = ((t_vec3f){
		src->data.cylinder.origin[0],
		src->data.cylinder.origin[1],
		src->data.cylinder.origin[2]});
	dst->p1 = ((t_vec3f){
		src->data.cylinder.origin[0] + src->data.cylinder.axis[0] * src->data.cylinder.height,
		src->data.cylinder.origin[1] + src->data.cylinder.axis[1] * src->data.cylinder.height,
		src->data.cylinder.origin[2] + src->data.cylinder.axis[2] * src->data.cylinder.height});
	dst->radius = (src->data.cylinder.diameter / 2.0f);
	return (SUCCESS);
}
int	copy_plane_to_primitive(const t_parsed_element *src, t_plane *dst)
{

	dst->normal.x = src->data.plane.normal[0];
	dst->normal.y = src->data.plane.normal[1];
	dst->normal.z = src->data.plane.normal[2];
	dst->d = -(dst->normal.x * src->data.plane.origin[0]
		+ dst->normal.y * src->data.plane.origin[1]
		+ dst->normal.z * src->data.plane.origin[2]);
	return (SUCCESS);
}