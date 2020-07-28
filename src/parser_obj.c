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

	if (split_count(arr) < 3)
		return (ERR_A);
	if (get_double(arr[1], &b) || b < 0 || b > 1)
		return (ERR_A);
	data->para.brightness = b;
	if (pars_color(arr[2], &(data->para.color)))
		return (ERR_A);
	return (SUCCESS);
	//ajouter le test de non-redefinition
}

int		pars_res(char **arr, t_data *data)
{
	int		n;

	if (split_count(arr) < 3)
		return (ERR_R);
	if (get_int(arr[1], &n) || n < 0 || n > 10000)
		return (ERR_R);
	data->para.res_width = n;
	if (get_int(arr[2], &n) || n < 0 || n > 2000)
		return (ERR_R);
	data->para.res_height = n;
	return (SUCCESS);
	//ajouter le test de non-redefinition
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
	return (SUCCESS);
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
	return (SUCCESS);
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
	if (pars_color(arr[3], &(sp->color)))
		return (ERR_SP);
	return (SUCCESS);
}

int		pars_pl(char **arr, t_data *data)
{
	t_pl	*pl;

	if (split_count(arr) < 4)
		return (ERR_PL);
	if (!(pl = malloc(sizeof(t_pl))))
		return (error(ERR_MALLOC));
	if ((list_addfront(&(data->lobj), (void *)pl, PLANE)))
		return (error(ERR_MALLOC));
	if (pars_point(arr[1], &(pl->point)))
		return (ERR_PL);
	if (pars_vector(arr[2], &(pl->norm)))
		return (ERR_PL);
	if (pars_color(arr[3], &(pl->color)))
		return (ERR_PL);
	return (SUCCESS);
}

int		pars_tr(char **arr, t_data *data)
{
	t_tr	*tr;

	if (split_count(arr) < 5)
		return (ERR_TR);
	if (!(tr = malloc(sizeof(t_tr))))
		return (error(ERR_MALLOC));
	if ((list_addfront(&(data->lobj), (void *)tr, TRIANGLE)))
		return (error(ERR_MALLOC));
	if (pars_point(arr[1], &(tr->p1)))
		return (ERR_TR);
	if (pars_point(arr[2], &(tr->p2)))
		return (ERR_TR);
	if (pars_point(arr[3], &(tr->p3)))
		return (ERR_TR);
	if (pars_color(arr[4], &(tr->color)))
		return (ERR_TR);
	return (SUCCESS);
}
