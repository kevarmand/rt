/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xpm_load.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 00:00:00 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/17 19:02:36 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "xpm.h"

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

static int	texture_alloc_pixels_vec3(t_texture *tex)
{
	t_vec3f	*pixels;
	int		count;

	count = tex->width * tex->height;
	pixels = malloc(sizeof(t_vec3f) * (size_t)count);
	if (!pixels)
		return (0);
	tex->pixels = pixels;
	tex->channels = 3;
	return (1);
}

static int	xpm_convert_texture(t_texture *tex, t_display *display,
				void *img, int is_bump)
{
	char	*addr;
	int		bpp;
	int		line_len;
	int		endian;
	int		bpp_line[2];

	if (!texture_alloc_pixels_vec3(tex))
		return (0);
	addr = mlx_get_data_addr(img, &bpp, &line_len, &endian);
	bpp_line[0] = line_len;
	bpp_line[1] = bpp;
	if (is_bump == 0)
		copy_albedo_from_mlx(tex, addr, bpp_line);
	else
	{
		bump_store_height_z(tex, addr, bpp_line);
		bump_bake_dxdy_xy(tex);
		bump_clear_z(tex);
	}
	(void)display;
	return (1);
}

static int	load_one_texture(t_texture *tex, t_display *display, int is_bump)
{
	void	*img;
	int		ok;

	if (!tex->path || !texture_has_xpm_ext(tex->path))
		return (0);
	ft_printf("\t\tLoading XPM file : %s\n", tex->path);
	img = mlx_xpm_file_to_image(display->mlx, tex->path,
			&tex->width, &tex->height);
	if (!img)
		return (0);
	if (is_bump == 0)
		ft_printf("\t\t\t-> Albedo conversion\n");
	else
		ft_printf("\t\t\t-> Bumpmap conversion\n");
	ok = xpm_convert_texture(tex, display, img, is_bump);
	ft_printf("\t\t\t-> Destruction d'image MLX\n\n");
	mlx_destroy_image(display->mlx, img);
	return (ok);
}

int	xpm_load(t_scene *scene, t_display *display)
{
	int	index;

	index = 0;
	while (index < scene->texture_count)
	{
		if (scene->textures[index].path)
			if (!load_one_texture(&scene->textures[index], display, 0))
				return (1);
		index++;
	}
	index = 0;
	while (index < scene->bumpmap_count)
	{
		if (scene->bumpmaps[index].path)
			if (!load_one_texture(&scene->bumpmaps[index], display, 1))
				return (1);
		index++;
	}
	return (0);
}
