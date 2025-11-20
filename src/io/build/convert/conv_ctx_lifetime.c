/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conv_ctx_lifetime.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 19:30:25 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/20 19:56:15 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "convert.h"
#include "io.h"
#include "errors.h"
#include "libft.h"

void	init_ctx(t_conv_ctx *cx)
{
	ft_memset(cx, 0, sizeof(t_conv_ctx));
}

void	free_ctx(t_conv_ctx *cx)
{
	vector_destroy(&cx->tex_v);
	hashmap_destroy(cx->tex_m, NULL);
	vector_destroy(&cx->mat_v);
	hashmap_destroy(cx->mat_m, NULL);
	vector_destroy(&cx->surf_v);
	vector_destroy(&cx->cam_v);
	vector_destroy(&cx->light_v);
	vector_destroy(&cx->plane_v);
	vector_destroy(&cx->obj_v);
}