/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_scene.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 14:49:35 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/01 17:54:18 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include "scene.h"
#include "errors.h"
#include "convert/convert.h"

int build_scene(t_scene *scene, t_scene_parsed *parsed)
{
	int			status;
	t_conv_ctx	ctx;
	
	conv_globals(parsed, scene);
	init_ctx(&ctx);
	status = conv_build_ctx(parsed, &ctx);
	if (status == SUCCESS)
		status = assemble_scene(&ctx, scene);
	//free_ctx(&ctx);
	return (status);
}
