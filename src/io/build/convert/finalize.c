#include "io.h"
#include "errors.h"
#include <stdlib.h>
#include "libft.h"
#include "scene.h"
#include "convert.h"


static int	finalize_prim_array(t_vector *vec,
			t_primitive **out_array, int *out_count)
{
	int		count = vector_size(vec);
	void	*src_ptr = vector_data(vec);

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

int	finalize_primitives(t_conv_ctx *cx, t_scene *scene)
{
	if (finalize_prim_array(&cx->obj_v,
			&scene->primitives, &scene->primitive_count) != SUCCESS)
		return (ERR_MALLOC);
	if (finalize_prim_array(&cx->plane_v,
			&scene->planes, &scene->plane_count) != SUCCESS)
		return (ERR_MALLOC);
	return (SUCCESS);
}