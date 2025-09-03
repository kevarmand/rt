/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmand <karmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 16:16:15 by karmand           #+#    #+#             */
/*   Updated: 2020/03/05 17:09:10 by karmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		get_int(char *str, int *n)
{
	int		i;
	int		sign;

	sign = 1;
	i = 0;
	(*n) = 0;
	if ((*str) && *str == '-')
	{
		sign = -1;
		str++;
	}
	while (*str && ft_isdigit(*str) && ++i < 8)
	{
		(*n) = (*n) * 10 + (*str) - '0';
		str++;
	}
	if (*str != 0)
		return (ERROR);
	(*n) = sign * (*n);
	return (SUCCESS);
}

int		get_double(char *str, double *d)
{
	int		sign;
	int		n;
	char	**deci;

	if (!(deci = ft_split(str, ".")))
		return (error(ERR_MALLOC));
	n = split_count(deci);
	if (n > 2 || n == 0)
		return (free_split(deci) - 1);
	if (get_int(deci[0], &n))
		return (free_split(deci) - 1);
	sign = 1;
	if (deci[0][0] == '-')
		sign = -1;
	(*d) = (double)n;
	if (deci[1] == 0)
		return (free_split(deci));
	if (get_int(deci[1], &n))
		return (free_split(deci) - 1);
	(*d) += ((double)sign * (double)n / (double)m_pow(10, ft_strlen(deci[1])));
	return (free_split(deci));
}

int		pars_color(char *str, t_color *color)
{
	char	**co;
	int		n;

	if (!(co = ft_split(str, ",")))
		return (error(ERR_MALLOC));
	if (split_count(co) != 3)
		return (free_split(co) + ERROR);
	if (get_int(co[0], &n) || n < 0 || n > 255)
		return (free_split(co) + ERROR);
	color->r = n;
	if (get_int(co[1], &n) || n < 0 || n > 255)
		return (free_split(co) + ERROR);
	color->g = n;
	if (get_int(co[2], &n) || n < 0 || n > 255)
		return (free_split(co) + ERROR);
	color->b = n;
	return (free_split(co));
}

int		pars_point(char *str, t_point *p)
{
	char	**po;
	double	d;

	if (!(po = ft_split(str, ",")))
		return (error(ERR_MALLOC));
	if (split_count(po) != 3)
		return (free_split(po) + ERROR);
	if (get_double(po[0], &d))
		return (free_split(po) + ERROR);
	p->x = d;
	if (get_double(po[1], &d))
		return (free_split(po) + ERROR);
	p->y = d;
	if (get_double(po[2], &d))
		return (free_split(po) + ERROR);
	p->z = d;
	return (free_split(po));
}

int		pars_vector(char *str, t_vector *v)
{
	char	**ve;
	double	d;

	if (!(ve = ft_split(str, ",")))
		return (error(ERR_MALLOC));
	if (split_count(ve) != 3)
		return (free_split(ve) + ERROR);
	if (get_double(ve[0], &d) || d > 1 || d < -1)
		return (free_split(ve) + ERROR);
	v->x = d;
	if (get_double(ve[1], &d) || d > 1 || d < -1)
		return (free_split(ve) + ERROR);
	v->y = d;
	if (get_double(ve[2], &d) || d > 1 || d < -1)
		return (free_split(ve) + ERROR);
	v->z = d;
	free_split(ve);
	if (v->x == 0 && v->y == 0 && v->z == 0)
		return (ERROR);
	return (SUCCESS);
}
