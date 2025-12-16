/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 20:40:32 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/16 22:54:33 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAMERA_H
# define CAMERA_H

# include "new_rt.h"
# include "display.h"
# include "vector.h"
# include "rt_config.h"
# include <math.h>

/***
 * Math utilities for camera operations
 */
t_vec3f	vec3f_rot_axis(t_vec3f vector, t_vec3f axis, float angle);
float	camera_mouse_yaw(const t_scene *scene,
			const t_camera *camera, int delta_x);
float	camera_mouse_pitch(const t_scene *scene,
			const t_camera *camera, int delta_y);
float	camera_roll_angle(const t_data *data, const t_mouse_state *mouse);

/***
 * Camera control handlers
 */

int		camera_handle_pan(t_data *data);
int		camera_handle_orbit(t_data *data);
int		camera_handle_look(t_data *data);
int		camera_handle_roll(t_data *data);
int		camera_handle_dolly(t_data *data);
int		camera_handle_level(t_data *data);

#endif