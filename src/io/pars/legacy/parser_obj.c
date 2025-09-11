/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_obj.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmand <karmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 16:52:13 by karmand           #+#    #+#             */
/*   Updated: 2020/03/05 22:59:03 by karmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		pars_a(char **arr, t_data *data)
{
	double	b;

	if ((data->test)[2] != 0)
		return (ERR_A_REDEF);
	(data->test)[2] = 1;
	if (split_count(arr) < 3)
		return (ERR_A);
	if (get_double(arr[1], &b) || b < 0 || b > 1)
		return (ERR_A);
	data->para.brightness = b;
	if (pars_color(arr[2], &(data->para.color)))
		return (ERR_A);
	return (SUCCESS);
}

int		pars_res(char **arr, t_data *data)
{
	int		n;

	if ((data->test)[1] != 0)
		return (ERR_R_REDEF);
	(data->test)[1] = 1;
	if (split_count(arr) < 3)
		return (ERR_R);
	if (get_int(arr[1], &n) || n < 0)
		return (ERR_R);
	data->para.res_width = n;
	if (get_int(arr[2], &n) || n < 0)
		return (ERR_R);
	data->para.res_height = n;
	return (SUCCESS);
}

int		pars_cam(char **arr, t_data *data)
{
	t_cam	*cam;

	if (split_count(arr) < 4)
		return (ERR_C);
	if (!(cam = malloc(sizeof(t_cam))))
		return (error(ERR_MALLOC));
	if (list_addfront(&(data->lcam), (void *)cam, CAM))
		return (error(ERR_MALLOC));
	if (pars_point(arr[1], &(cam->point)))
		return (ERR_C);
	if (pars_vector(arr[2], &(cam->dir)))
		return (ERR_C);
	if (get_double(arr[3], &(cam->fov)))
		return (ERR_C);
	if (cam->fov >= 180 || cam->fov <= 0)
		return (ERR_C);
	(cam->fov) *= 0.01745329251;
	return (init_cam(cam, data));
}

int		pars_light(char **arr, t_data *data)
{
	t_light	*light;
	double	d;

	if (split_count(arr) < 4)
		return (ERR_L);
	if (!(light = malloc(sizeof(t_light))))
		return (error(ERR_MALLOC));
	if (list_addfront(&(data->llight), (void *)light, LIGHT))
		return (error(ERR_MALLOC));
	if (pars_point(arr[1], &(light->point)))
		return (ERR_L);
	if (get_double(arr[2], &d) || d > 1 || d < 0)
		return (ERR_L);
	light->brightness = d;
	if (pars_color(arr[3], &(light->color)))
		return (ERR_L);
	return (init_light(light));
}

int		pars_sp(char **arr, t_data *data)
{
	t_sp	*sp;

	if (split_count(arr) < 4)
		return (ERR_SP);
	if (!(sp = malloc(sizeof(t_sp))))
		return (error(ERR_MALLOC));
	if (list_addfront(&(data->lobj), (void *)sp, SPHERE))
		return (error(ERR_MALLOC));
	if (pars_point(arr[1], &(sp->centre)))
		return (ERR_SP);
	if (get_double(arr[2], &(sp->rayon)) || sp->rayon <= 0)
		return (ERR_SP);
	(sp->rayon) /= 2;
	if (pars_color(arr[3], &(sp->color)))
		return (ERR_SP);
	return (init_sp(sp));
}
