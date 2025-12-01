/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_xpm.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 14:46:28 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/01 17:41:15 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "display.h"
#include "scene.h"
#include "libft.h"
#include "mlx.h"
#include "color.h"

static int	texture_has_xpm_ext(const char *path)
{
	size_t	len;

	len = ft_strlen(path);
	if (len < 4)
		return (0);
	if (path[len - 4] != '.')
		return (0);
	if (path[len - 3] != 'x')
		return (0);
	if (path[len - 2] != 'p')
		return (0);
	if (path[len - 1] != 'm')
		return (0);
	return (1);
}

static int	texture_alloc_pixels(t_texture *tex)
{
	t_vec3f	*pixels;
	int		count;

	count = tex->width * tex->height;
	pixels = malloc(sizeof(t_vec3f) * count);
	if (!pixels)
		return (0);
	tex->pixels = pixels;
	tex->channels = 3;
	return (1);
}

static void	texture_copy_from_mlx(t_texture *tex, void *img, void *mlx)
{
	char	*addr;
	int		bpp;
	int		line_len;
	int		endian;
	int		x;
	int		y;
	int		rgb[3];

	addr = mlx_get_data_addr(img, &bpp, &line_len, &endian);
	y = 0;
	while (y < tex->height)
	{
		x = 0;
		while (x < tex->width)
		{
			int	pix;
			int	index;
			t_vec3f	*dst;

			pix = *(int *)(addr + y * line_len + x * (bpp / 8));
			rgb[0] = (pix >> 16) & 0xFF;
			rgb[1] = (pix >> 8) & 0xFF;
			rgb[2] = pix & 0xFF;
			index = y * tex->width + x;
			dst = &((t_vec3f *)tex->pixels)[index];
			rgb8_to_linear_vec(rgb, dst);
			x++;
		}
		y++;
	}
	mlx_destroy_image(mlx, img);
}
#include <stdio.h>

static int	load_one_texture(t_texture *tex, t_display *display)
{
	void	*img;

	if (!tex->path || !texture_has_xpm_ext(tex->path))
		return (0);
	printf("Loading texture from XPM file: %s\n", tex->path);
	img = mlx_xpm_file_to_image(display->mlx, tex->path,
			&tex->width, &tex->height);
	if (!img)
		return (0);
	if (!texture_alloc_pixels(tex))
	{
		mlx_destroy_image(display->mlx, img);
		return (0);
	}
	texture_copy_from_mlx(tex, img, display->mlx);
	return (1);
}

int	load_scene_textures(t_scene *scene, t_display *display)
{
	int	i;

	i = 0;
	while (i < scene->texture_count)
	{
		printf("Loading texture %d/%d: %s\n", i + 1, scene->texture_count, scene->textures[i].path);
		if (scene->textures[i].path)
		{
			if (!load_one_texture(&scene->textures[i], display))
				return (1);
		}
		i++;
	}
	return (0);
}
