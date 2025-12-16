/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cam_ctrl.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 15:16:40 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/16 21:06:56 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAM_CTRL_H
# define CAM_CTRL_H
# include "vector.h"

typedef enum e_cam_mode
{
	CAM_MODE_STANDARD,
	CAM_MODE_ROLL
}	t_cam_mode;

typedef struct s_cam_ctrl
{
	int			flag_level;
	float		nav_scale;
	float		nav_mul;
	t_vec3f		center;
	float		roll_prev_angle;
	t_cam_mode	mode;
}	t_cam_ctrl;

#endif