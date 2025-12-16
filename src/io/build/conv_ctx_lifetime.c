/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conv_ctx_lifetime.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 19:30:25 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/16 16:30:56 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "convert/convert.h"
#include "io.h"
#include "errors.h"
#include "libft.h"
#include "scene.h"

int	init_ctx(t_conv_ctx *cx)
{
	ft_memset(cx, 0, sizeof(t_conv_ctx));
	cx->mat_m = hashmap_create(256);
	if (cx->mat_m == NULL)
		return (ERR_MALLOC);
	if (vector_init(&cx->mat_v, 0, sizeof(t_material)))
		return (ERR_MALLOC);
	if (vector_init(&cx->surf_v, 0, sizeof(t_surface_map)))
		return (ERR_MALLOC);
	if (vector_init(&cx->cam_v, 0, sizeof(t_camera)))
		return (ERR_MALLOC);
	if (vector_init(&cx->light_v, 0, sizeof(t_light)))
		return (ERR_MALLOC);
	if (vector_init(&cx->plane_v, 0, sizeof(t_primitive)))
		return (ERR_MALLOC);
	if (vector_init(&cx->obj_v, 0, sizeof(t_primitive)))
		return (ERR_MALLOC);
	return (SUCCESS);
}

void	free_ctx(t_conv_ctx *cx)
{
	vector_destroy(&cx->mat_v);
	hashmap_destroy(cx->mat_m, NULL);
	vector_destroy(&cx->surf_v);
	vector_destroy(&cx->cam_v);
	vector_destroy(&cx->light_v);
	vector_destroy(&cx->plane_v);
	vector_destroy(&cx->obj_v);
}
