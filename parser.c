/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmand <karmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/10 13:27:41 by karmand           #+#    #+#             */
/*   Updated: 2020/02/26 19:02:02 by karmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

//retour du pqrsong => nombre d'element dans ma liste pour cree la liste.
//indexer
// Erreur de parsing 
/* -1 => imposible d ouvrir le fichier
*/

int		parse_color(int fd, t_color *color)
{
	int		n;
	int		c;

	c = get_int(fd, &n);
	if (c != ',' || n > 255 || n < 0)
		return (ERROR_PARS_COLOR);
	color->r = n;
	c = get_int(fd, &n);
	if (c != ',' || n > 255 || n < 0)
		return (ERROR_PARS_COLOR);
	color->g = n;
	c = get_int(fd, &n);
	if ((ft_iswp(c) != 1 && c != '\n') || n > 255 || n < 0)
		return (ERROR_PARS_COLOR);
	color->b = n;	
	return (SUCCESS);
}

int		pars_point(int fd, t_point *point)
{
	double	d;
	char	c;

	c = get_double(fd, &d);
	if (c != ',')
		return (ERROR_PARS_POINT);
	point->x = d;
	c = get_double(fd, &d); 
	if (c != ',')
		return (ERROR_PARS_POINT);
	point->y = d;
	c = get_double(fd, &d);
	if ((ft_iswp(c) != 1 && c != '\n'))
		return (ERROR_PARS_POINT);
	point->z = d;
	return (SUCCESS);
}

int		pars_vector(int fd, t_vector *v)
{
	int		err;

	if ((err = pars_point(fd, (t_point *)v)))
		return (ERROR_PARS_VECTOR);
	if (v->x > 1 || v->x < -1 || v->y > 1 || v->y < -1 ||
		 v->z > 1 || v->z < -1)
		return (ERROR_PARS_VECTOR);
	test_print_point((t_point *)v);
	return (SUCCESS);
}

int		pars_check(int fd, char c, t_data *data)
{
	if (c == 'A')
		return (parse_a(fd, data));
	if (c == 'R')
		return (parse_res(fd, data));
	if (c == 't')
	{
		if (!read(fd,&c, 1))
			return (ERROR_READ);
		if (c == 'r')
			return (parse_tr(fd, data));
		else
			return (ERROR_PARS_TYPE);
	}
	if (c == 'c')
	{
		if (!read(fd,&c, 1))
			return (ERROR_READ);
		if (c == 'y')
			return (4);
		if (c == ' ' || c == '\t')
			return (parse_cam(fd, data));
		else
			return (ERROR_PARS_TYPE);
	}
	if (c == 'l')
		return (parse_light(fd, data));
	if (c == 'p')
	{
		if (!read(fd,&c, 1))
			return (ERROR_READ);
		if (c == 'l')
			return (parse_pl(fd, data));
		else
			return (ERROR_PARS_TYPE);
	}
	if (c == 's')
	{
		if (!read(fd,&c, 1))
			return (ERROR_READ);
		if (c == 'q')
			return (8);
		if (c == 'p')
			return (parse_sp(fd, data));
		else
			return (ERROR_PARS_TYPE);
	}
	return (ERROR_PARS_TYPE);
}

int		parsing(int fd, t_data *data)
{	
	int		err;
	int		r;
	char	c;

	r = read(fd, &c, 1);
	int test = -1;
	while (r && c == '\n')
		r = read(fd, &c, 1);
	while (r)
	{
		test++;
		if ((err = pars_check(fd, c, data))<0)
			return (err);
		printf("ligne %i : %i\n", test, err);
		r = read(fd, &c, 1);
		
		while (r && c == '\n')
			r = read(fd, &c, 1);
	}
	return (SUCCESS);
}
