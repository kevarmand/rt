#include "io.h"
#include "error_codes.h"
#include <stdlib.h>
#include "scene.h"

t_vec3f	vec3f_sub(t_vec3f a, t_vec3f b);

//Onconvertit les 3 point en 1 point +2 edges
int copy_obj_triangle(const t_parsed_element *src, t_triangle *dst)
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