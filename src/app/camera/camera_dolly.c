/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_dolly.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 22:32:44 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/16 22:35:44 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "camera.h"

typedef struct dolly_ctx
{
	t_display		*display;
	t_mouse_state	*mouse;
	t_camera		*camera;
	t_cam_ctrl		*ctrl;
	int				scroll;
	float			scale;
	float			step;
}	t_dolly_ctx;

int	camera_handle_dolly(t_data *data)
{
	t_dolly_ctx	ctx;

	ctx.display = &data->display;
	ctx.mouse = &ctx.display->mouse;
	ctx.scroll = ctx.mouse->scroll_delta;
	if (ctx.scroll == 0)
		return (0);
	ctx.ctrl = &ctx.display->cam_ctrl;
	ctx.camera = &data->scene.cameras[ctx.display->current_cam];
	ctx.scale = ctx.ctrl->nav_scale * ctx.ctrl->nav_mul;
	ctx.step = ((float)ctx.scroll * (float)SCROLL_PIXEL)
		/ (float)data->scene.resolution_height;
	ctx.camera->origin = vec3f_add(ctx.camera->origin,
			vec3f_scale(ctx.camera->forward, ctx.step * ctx.scale));
	ctx.mouse->scroll_delta = 0;
	return (1);
}
