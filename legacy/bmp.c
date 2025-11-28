/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bmp.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmand <karmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 16:16:15 by karmand           #+#    #+#             */
/*   Updated: 2020/03/05 17:09:10 by karmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		bmp_header(int fd, t_data *data)
{
	unsigned char	header[54];
	int				size;

	ft_bzero(header, 54);
	size = 54 + (data->para).res_width * (data->para).res_height;
	header[0] = 'B';
	header[1] = 'M';
	*((int *)&header[2]) = size;
	header[10] = (unsigned char)54;
	header[14] = (unsigned char)40;
	*((int *)&header[18]) = (data->para).res_width;
	*((int *)&header[22]) = (data->para).res_height;
	header[26] = (unsigned char)1;
	header[28] = (unsigned char)24;
	write(fd, header, 54);
	return (0);
}

int		bmp_size(int n)
{
	if (n % 4)
		return (n + (4 - (n % 4)));
	return (n);
}

int		bmp_core(int fd, t_data *data)
{
	int		x;
	int		y;
	int		n;
	int		n1;
	char	*buf;

	y = (data->para).res_height;
	n1 = bmp_size(3 * data->para.res_width);
	buf = malloc(n1);
	ft_bzero(buf, n1);
	while (--y >= 0)
	{
		x = -1;
		while (++x < (data->para).res_width)
		{
			n = *(int *)(data->view->addr_ptr + ((y * data->para.res_width + x)
			* data->view->bpp / 8));
			buf[3 * x] = (unsigned char)(n % 256);
			n /= 256;
			buf[3 * x + 1] = (unsigned char)(n % 256);
			buf[3 * x + 2] = (unsigned char)((n / 256) % 256);
		}
		write(fd, buf, n1);
	}
	return (ft_free(buf));
}

int		bmp(t_data *data, char *name)
{
	int		fd;
	char	**arr;

	arr = ft_split(name, ".");
	fd = split_count(arr) - 1;
	free(arr[fd]);
	arr[fd] = "bmp";
	name = ft_strjoin(arr, ".");
	arr[fd] = 0;
	free_split(arr);
	fd = open(name, O_WRONLY | O_CREAT, 0644);
	if (fd <= 0)
		return (ft_free(name) + error(ERR_BMP));
	bmp_header(fd, data);
	bmp_core(fd, data);
	close(fd);
	free(name);
	return (0);
}
