/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_fast2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 22:28:23 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/17 22:40:42 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include "render.h"
#include "scene.h"
#include "rt_config.h"
#include "new_rt.h"
#include "errors.h"

typedef struct s_coef
{
	t_vec3f	c00;
	t_vec3f	c01;
	t_vec3f	c10;
	t_vec3f	c11;
}	t_coef;

void	init_coef(t_coef *coef, t_tile *tile, int x, int y)
{
	coef->c00 = tile->hdr_pixels[(y * tile->pixel_width) + x];
	coef->c01 = tile->hdr_pixels[(y * tile->pixel_width) + (x + 3)];
	coef->c10 = tile->hdr_pixels[((y + 3) * tile->pixel_width) + x];
	coef->c11 = tile->hdr_pixels[((y + 3) * tile->pixel_width) + (x + 3)];
}

static void	bilinear_color(const t_coef *coef, int i, int j, t_vec3f *out)
{
	t_vec3f	cx0;
	t_vec3f	cx1;
	float	tx;
	float	ty;

	tx = (float)i / 3.0f;
	ty = (float)j / 3.0f;
	cx0 = vec3f_add(vec3f_scale(coef->c00, 1.0f - tx),
			vec3f_scale(coef->c01, tx));
	cx1 = vec3f_add(vec3f_scale(coef->c10, 1.0f - tx),
			vec3f_scale(coef->c11, tx));
	*out = vec3f_add(vec3f_scale(cx0, 1.0f - ty),
			vec3f_scale(cx1, ty));
}

static void	interpolate_block(t_tile *tile, int x, int y)
{
	t_coef	coef;
	int		i;
	int		j;
	t_vec3f	color;

	init_coef(&coef, tile, x, y);
	j = 0;
	while (j < 3)
	{
		i = 0;
		while (i < 3)
		{
			bilinear_color(&coef, i, j, &color);
			tile->hdr_pixels[(y + j) * tile->pixel_width + (x + i)] = color;
			i++;
		}
		j++;
	}
}

void	interpolate_tile_blocks(t_tile *tile,
			int *last_row, int *last_col)
{
	int	y;
	int	x;

	*last_row = -1;
	*last_col = -1;
	y = 0;
	while (y + 3 < tile->pixel_height)
	{
		x = 0;
		while (x + 3 < tile->pixel_width)
		{
			interpolate_block(tile, x, y);
			*last_col = x + 2;
			x += 3;
		}
		*last_row = y + 2;
		y += 3;
	}
}
