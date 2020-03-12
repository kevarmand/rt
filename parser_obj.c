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

int		parse_a(int fd, t_data *data)
{
	char c;
	
	c = get_double(fd, &(data->para.brightness));
	if (!ft_iswp(c))
		return (ERROR_PARS_TYPE);
	return (parse_color(fd, &(data->para.color)));
}

int		parse_res(int fd, t_data *data)
{
	char c;
	c = get_int(fd, &(data->para.res_width));
	if (!ft_iswp(c))
		return (ERROR_PARS_TYPE);

	c = get_int(fd, &(data->para.res_height));
	if (!ft_iswp(c) && c != '\n')
		return (ERROR_PARS_TYPE);
	return (SUCCESS);
}

int		parse_cam(int fd, t_data *data)
{
	char	c;
	int		err;
	t_cam	*cam;

	if (!(cam = malloc(sizeof(t_cam))))
		return (ERROR_MALLOC);
	if ((err = list_addfront(&(data->lcam), (void *)cam, CAM)))
		return (ERROR_MALLOC);
	if ((err = pars_point(fd, &(cam->point))))
		return (err);
	if ((err = pars_vector(fd, &(cam->dir))))
		return (err);
	c = get_double(fd, &(cam->fov));
	if (cam->fov >= 180 || cam->fov <= 0)
		return (ERROR_FOV);
	if (!ft_iswp(c) && c != '\n')
		return (ERROR_FOV);
	return (SUCCESS);
}

int		parse_light(int fd, t_data *data)
{
	char	c;
	int		err;
	t_light	*light;

	if (!(light = malloc(sizeof(t_light))))
		return (ERROR_MALLOC);
	if ((err = list_addfront(&(data->llight), (void *)light, LIGHT)))
		return (ERROR_MALLOC);
	if ((err = pars_point(fd, &(light->point))))
		return (err);
	c = get_double(fd, &(light->brightness));
	if (light->brightness > 1 || light->brightness < 0)
		return (ERROR_BRIGHTNESS);
	if (!ft_iswp(c))
		return (ERROR_BRIGHTNESS);
	if ((err = parse_color(fd, &(light->color))))
		return (err);
	return (SUCCESS);
}

int		parse_sp(int fd, t_data *data)
{
	char	c;
	int		err;
	t_sp	*sp;

	if (!(sp = malloc(sizeof(t_sp))))
		return (ERROR_MALLOC);
	if ((err = list_addfront(&(data->lobj), (void *)sp, SPHERE)))
		return (ERROR_MALLOC);
	if ((err = pars_point(fd, &(sp->centre))))
		return (err);
	c = get_double(fd, &(sp->rayon));
	if (sp->rayon <= 0)
		return (ERROR_RAYON);
	if (!ft_iswp(c))
		return (ERROR_RAYON);
	return (parse_color(fd, &(sp->color)));
}

int		parse_pl(int fd, t_data *data)
{
	char	c;
	int		err;
	t_pl	*pl;
	
	printf("parseplaa \n");
	if (!(pl = malloc(sizeof(t_pl))))
		return (ERROR_MALLOC);
	if ((err = list_addfront(&(data->lobj), (void *)pl, PLANE)))
		return (ERROR_MALLOC);
	if ((err = pars_point(fd, &(pl->point))))
		return (err);
	if ((err = pars_vector(fd, &(pl->norm))))
		return (err);
	return (parse_color(fd, &(pl->color)));
}

int		parse_tr(int fd, t_data *data)
{
	char	c;
	int		err;
	t_tr	*tr;

	if (!(tr = malloc(sizeof(t_tr))))
		return (ERROR_MALLOC);
	if ((err = list_addfront(&(data->lobj), (void *)tr, TRIANGLE)))
		return (ERROR_MALLOC);
	if ((err = pars_point(fd, &(tr->p1))))
		return (err);
	if ((err = pars_point(fd, &(tr->p2))))
		return (err);
	if ((err = pars_point(fd, &(tr->p3))))
		return (err);
	return (parse_color(fd, &(tr->color)));
}
