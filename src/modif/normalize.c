/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normalize.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmand <karmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 16:16:15 by karmand           #+#    #+#             */
/*   Updated: 2020/03/05 17:09:10 by karmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		norm_pl(t_pl *pl)
{
	pl->a = pl->norm.x;
	pl->b = pl->norm.y;
	pl->c = pl->norm.z;
	pl->d = -(pl->a * pl->point.x);
	pl->d -= (pl->b * pl->point.y);
	pl->d -= (pl->c * pl->point.z);
	return (SUCCESS);
}

int		norm_sq(t_sq *sq)
{
	sq->a = sq->norm.x;
	sq->b = sq->norm.y;
	sq->c = sq->norm.z;
	sq->d = -(sq->a * sq->point.x);
	sq->d -= (sq->b * sq->point.y);
	sq->d -= (sq->c * sq->point.z);
	return (SUCCESS);
}

int		norm_tr(t_tr *tr)
{
	tr->a = tr->norm.x;
	tr->b = tr->norm.y;
	tr->c = tr->norm.z;
	tr->d = -(tr->a * tr->p1.x);
	tr->d -= (tr->b * tr->p1.y);
	tr->d -= (tr->c * tr->p1.z);
	return (SUCCESS);
}
