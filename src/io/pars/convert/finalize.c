#include "io.h"
#include "error_codes.h"
#include <stdlib.h>
#include "libft.h"
#include "scene.h"
#include "convert.h"


static int	finalize_prim_array(const t_vector *vec,
			t_primitive **out_array, int *out_count)
{
	const int	count = vector_size(vec);
	const void	*src_ptr = vector_data(vec);

	*out_count = count;
	*out_array = NULL;
	if (count <= 0)
		return (SUCCESS);
	*out_array = malloc(sizeof(t_primitive) * count);
	if (!*out_array)
		return (ERR_MALLOC);
	ft_memcpy(*out_array, src_ptr, sizeof(t_primitive) * count);
	return (SUCCESS);
}

int	finalize_primitives(const t_conv_ctx *cx, t_scene *scene)
{
	if (finalize_prim_array(&cx->obj_v,
			&scene->objects, &scene->object_count) != SUCCESS)
		return (ERR_MALLOC);
	if (finalize_prim_array(&cx->plane_v,
			&scene->planes, &scene->plane_count) != SUCCESS)
		return (ERR_MALLOC);
	return (SUCCESS);
}