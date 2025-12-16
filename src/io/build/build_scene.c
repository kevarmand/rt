/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_scene.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 14:49:35 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/16 16:26:02 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include "scene.h"
#include "errors.h"
#include "convert/convert.h"

int	build_scene(t_scene *scene, t_scene_parsed *parsed)
{
	int			status;
	t_conv_ctx	ctx;

	conv_globals(parsed, scene);
	status = init_ctx(&ctx);
	if (status == SUCCESS)
		status = conv_build_ctx(parsed, &ctx);
	if (status == SUCCESS)
		status = assemble_scene(&ctx, scene);
	if (status == SUCCESS)
		status = build_textures(scene, &parsed->textures);
	free_ctx(&ctx);
	return (status);
}
