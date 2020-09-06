/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmand <karmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 16:16:15 by karmand           #+#    #+#             */
/*   Updated: 2020/03/05 17:09:10 by karmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		render(t_data *data)
{
	cam_gen(data, (t_cam *)(((data->select).cam)->obj));
	mlx_put_image_to_window(data->mlx_ptr, data->win_ptr,
	(data->view)->img_ptr, 0, 0);
	return (SUCCESS);
}
