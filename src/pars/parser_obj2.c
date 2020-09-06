/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_obj2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmand <karmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 16:52:13 by karmand           #+#    #+#             */
/*   Updated: 2020/03/05 22:59:03 by karmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

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
	return (init_pl(pl));
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
	return (init_tr(tr));
}

int		pars_cy(char **arr, t_data *data)
{
	t_cy	*cy;

	if (split_count(arr) < 6)
		return (ERR_CY);
	if (!(cy = malloc(sizeof(t_cy))))
		return (error(ERR_MALLOC));
	if ((list_addfront(&(data->lobj), (void *)cy, CYLINDER)))
		return (error(ERR_MALLOC));
	if (pars_point(arr[1], &(cy->point)))
		return (ERR_CY);
	if (pars_vector(arr[2], &(cy->dir)))
		return (ERR_CY);
	if (get_double(arr[3], &(cy->rayon)) || (cy->rayon) <= 0)
		return (ERR_CY);
	(cy->rayon) /= 2;
	if (get_double(arr[4], &(cy->height)) || (cy->height) <= 0)
		return (ERR_CY);
	if (pars_color(arr[5], &(cy->color)))
		return (ERR_CY);
	return (init_cy(cy));
}

int		pars_sq(char **arr, t_data *data)
{
	t_sq	*sq;

	if (split_count(arr) < 5)
		return (ERR_CY);
	if (!(sq = malloc(sizeof(t_sq))))
		return (error(ERR_MALLOC));
	if ((list_addfront(&(data->lobj), (void *)sq, SQUARE)))
		return (error(ERR_MALLOC));
	if (pars_point(arr[1], &(sq->point)))
		return (ERR_SQ);
	if (pars_vector(arr[2], &(sq->norm)))
		return (ERR_SQ);
	if (get_double(arr[3], &(sq->height)) || (sq->height) <= 0)
		return (ERR_SQ);
	if (pars_color(arr[4], &(sq->color)))
		return (ERR_SQ);
	return (init_sq(sq));
}
