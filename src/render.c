/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 16:16:15 by karmand           #+#    #+#             */
/*   Updated: 2025/08/03 21:48:53 by kearmand         ###   ########.fr       */
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
