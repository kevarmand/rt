/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_xpm.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 00:00:00 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/17 04:05:06 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scene.h"
#include "display.h"
#include "mlx.h"
#include <stdlib.h>
#include <math.h>
#include "libft.h"
#include "color.h"
#include <stdio.h>

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

static void	extract_rgb(int pixel_value, int rgb[3])
{
	rgb[0] = (pixel_value >> 16) & 0xFF;
	rgb[1] = (pixel_value >> 8) & 0xFF;
	rgb[2] = pixel_value & 0xFF;
}

static float	rgb8_to_height(int rgb[3])
{
	return ((float)(rgb[0] + rgb[1] + rgb[2]) * (1.0f / (3.0f * 255.0f)));
}

static int	wrap_index(int value, int size)
{
	value = value % size;
	if (value < 0)
		value += size;
	return (value);
}

static void	bump_store_height_z(t_texture *tex, char *addr,
				int bpp_line[2])
{
	int		pos_y;
	int		pos_x;
	int		rgb[3];
	int		pixel_value;
	int		texel_index;
	int		line_len;
	int		bytespp;
	t_vec3f	*pixels;

	line_len = bpp_line[0];
	bytespp = bpp_line[1] / 8;
	pixels = (t_vec3f *)tex->pixels;
	pos_y = 0;
	while (pos_y < tex->height)
	{
		pos_x = 0;
		while (pos_x < tex->width)
		{
			pixel_value = *(int *)(addr + pos_y * line_len + pos_x * bytespp);
			extract_rgb(pixel_value, rgb);
			texel_index = pos_y * tex->width + pos_x;
			pixels[texel_index] = (t_vec3f){0.0f, 0.0f, rgb8_to_height(rgb)};
			pos_x++;
		}
		pos_y++;
	}
}

static float	height_at_z(const t_vec3f *pixels, int width, int x, int y)
{
	return (pixels[y * width + x][2]);
}

static void	bump_bake_dxdy_xy(t_texture *tex)
{
	int		pos_y;
	int		pos_x;
	int		width;
	int		height;
	int		xm;
	int		xp;
	int		ym;
	int		yp;
	float	dx;
	float	dy;
	t_vec3f	*pixels;

	width = tex->width;
	height = tex->height;
	pixels = (t_vec3f *)tex->pixels;
	pos_y = 0;
	while (pos_y < height)
	{
		ym = wrap_index(pos_y - 1, height);
		yp = wrap_index(pos_y + 1, height);
		pos_x = 0;
		while (pos_x < width)
		{
			xm = wrap_index(pos_x - 1, width);
			xp = wrap_index(pos_x + 1, width);
			dx = (height_at_z(pixels, width, xp, pos_y)
					- height_at_z(pixels, width, xm, pos_y)) * 0.5f;
			dy = (height_at_z(pixels, width, pos_x, yp)
					- height_at_z(pixels, width, pos_x, ym)) * 0.5f;
			pixels[pos_y * width + pos_x][0] = dx;
			pixels[pos_y * width + pos_x][1] = dy;
			pos_x++;
		}
		pos_y++;
	}
}

static void	bump_clear_z(t_texture *tex)
{
	int		texel_index;
	int		count;
	t_vec3f	*pixels;

	pixels = (t_vec3f *)tex->pixels;
	count = tex->width * tex->height;
	texel_index = 0;
	while (texel_index < count)
	{
		pixels[texel_index][2] = 0.0f;
		texel_index++;
	}
}

static void	copy_albedo_from_mlx(t_texture *tex, char *addr,
				int bpp_line[2])
{
	int		pos_y;
	int		pos_x;
	int		rgb[3];
	int		pixel_value;
	int		texel_index;
	int		line_len;
	int		bytespp;
	t_vec3f	*pixels;

	line_len = bpp_line[0];
	bytespp = bpp_line[1] / 8;
	pixels = (t_vec3f *)tex->pixels;
	pos_y = 0;
	while (pos_y < tex->height)
	{
		pos_x = 0;
		while (pos_x < tex->width)
		{
			pixel_value = *(int *)(addr + pos_y * line_len + pos_x * bytespp);
			extract_rgb(pixel_value, rgb);
			texel_index = pos_y * tex->width + pos_x;
			rgb8_to_linear_vec(rgb, &pixels[texel_index]);
			pos_x++;
		}
		pos_y++;
	}
}

void bump_print_texture(t_texture *tex)
{
	int		pos_y;
	int		pos_x;
	int		width;
	int		height;
	t_vec3f	*pixels;

	width = tex->width;
	height = tex->height;
	pixels = (t_vec3f *)tex->pixels;
	pos_y = 0;
	while (pos_y < height)
	{
		pos_x = 0;
		while (pos_x < width)
		{
			printf("(%.2f,%.2f) ", pixels[pos_y * width + pos_x][0],
				pixels[pos_y * width + pos_x][1]);
			pos_x++;
		}
		ft_printf("\n");
		pos_y++;
	}
}

//petit log provisoire

static int	load_one_texture(t_texture *tex, t_display *display, int is_bump)
{
	void	*img;
	char	*addr;
	int		bpp;
	int		line_len;
	int		endian;
	int		bpp_line[2];

	if (!tex->path || !texture_has_xpm_ext(tex->path))
		return (0);
	ft_printf("\t\tLoading XPM file : %s\n", tex->path);
	img = mlx_xpm_file_to_image(display->mlx, tex->path,
			&tex->width, &tex->height);
	if (!img)
		return (0);
	if (!texture_alloc_pixels_vec3(tex))
		return (mlx_destroy_image(display->mlx, img), 0);
	addr = mlx_get_data_addr(img, &bpp, &line_len, &endian);
	bpp_line[0] = line_len;
	bpp_line[1] = bpp;
	if (is_bump == 0)
	{
		ft_printf("\t\t\t-> Albedo conversion\n");
		copy_albedo_from_mlx(tex, addr, bpp_line);
	}
	else
	{
		ft_printf("\t\t\t-> Bumpmap conversion\n");
		bump_store_height_z(tex, addr, bpp_line);
		bump_bake_dxdy_xy(tex);
		bump_clear_z(tex);
		//bump_print_texture(tex);
	}
	ft_printf("\t\t\t-> Destruction d'image MLX\n\n");
	mlx_destroy_image(display->mlx, img);
	return (1);
}

int	load_scene_textures(t_scene *scene, t_display *display)
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
