/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conv_build_ctx.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 16:33:45 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/16 16:34:03 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "convert.h"
#include "io.h"
#include "errors.h"

int	conv_build_ctx(t_scene_parsed *parsed, t_conv_ctx *ctx)
{
	int	status;

	conv_count_all(parsed, ctx);
	status = conv_cameras(parsed, ctx);
	if (status == SUCCESS)
		status = conv_lights(parsed, ctx);
	if (status == SUCCESS)
		status = conv_primitives(parsed, ctx);
	return (status);
}
