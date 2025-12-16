/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_pan.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 22:37:02 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/16 22:43:10 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "camera.h"

typedef struct s_pan_ctx
{
	t_display		*display;
	t_mouse_state	*mouse;
	t_camera		*camera;
	t_vec3f			up;
	float			scale;
	float			step_x;
	float			step_y;
	int				delta_x;
	int				delta_y;
}	t_pan_ctx;

int	camera_handle_pan(t_data *data)
{
	t_pan_ctx	ctx;

	ctx.display = &data->display;
	ctx.mouse = &ctx.display->mouse;
	if (ctx.mouse->is_down == 0 || ctx.mouse->button != MOUSE_BUTTON_RIGHT)
		return (0);
	ctx.delta_x = ctx.mouse->current_x - ctx.mouse->anchor_x;
	ctx.delta_y = ctx.mouse->current_y - ctx.mouse->anchor_y;
	if (ctx.delta_x == 0 && ctx.delta_y == 0)
		return (0);
	ctx.mouse->anchor_x = ctx.mouse->current_x;
	ctx.mouse->anchor_y = ctx.mouse->current_y;
	ctx.camera = &data->scene.cameras[ctx.display->current_cam];
	ctx.up = vec3f_cross(ctx.camera->right, ctx.camera->forward);
	ctx.up = vec3f_normalize(ctx.up);
	ctx.scale = ctx.display->cam_ctrl.nav_scale * ctx.display->cam_ctrl.nav_mul;
	ctx.step_x = (float)ctx.delta_x / (float)data->scene.resolution_width;
	ctx.step_y = (float)ctx.delta_y / (float)data->scene.resolution_height;
	ctx.camera->origin = vec3f_sub(ctx.camera->origin,
			vec3f_scale(ctx.camera->right, ctx.step_x * ctx.scale));
	ctx.camera->origin = vec3f_add(ctx.camera->origin,
			vec3f_scale(ctx.up, ctx.step_y * ctx.scale));
	return (1);
}
