/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ctx.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 16:03:20 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/21 16:05:35 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../io/build/convert/convert.h"
#include <stdio.h>

//on affiche le contenue de ctx pour debug
void print_ctx(t_conv_ctx *cx)
{
	printf("Conversion Context:\n");
	printf(" Cameras: %d\n", cx->camera_count);
	printf(" Lights: %d\n", cx->light_count);
	printf(" Objects: %d\n", cx->object_count);
	printf(" Planes: %d\n", cx->plane_count);
	printf(" Textures: %d\n", (int)vector_size(&cx->tex_v));
	printf(" Materials: %d\n", (int)vector_size(&cx->mat_v));
	printf(" Surfaces: %d\n", (int)vector_size(&cx->surf_v));
}