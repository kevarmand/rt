/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xpm_bump.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 17:59:54 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/17 18:50:20 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "xpm.h"

typedef struct s_bump_store_ctx
{
	int		pos_y;
	int		pos_x;
	int		rgb[3];
	int		pixel_value;
	int		texel_index;
	int		line_len;
	int		bytespp;
	t_vec3f	*pixels;
}	t_bump_store_ctx;

void	bump_store_height_z(t_texture *tex, char *addr,
				int bpp_line[2])
{
	t_bump_store_ctx	ctx;

	ctx.line_len = bpp_line[0];
	ctx.bytespp = bpp_line[1] / 8;
	ctx.pixels = (t_vec3f *)tex->pixels;
	ctx.pos_y = 0;
	while (ctx.pos_y < tex->height)
	{
		ctx.pos_x = 0;
		while (ctx.pos_x < tex->width)
		{
			ctx.pixel_value = *(int *)(addr + ctx.pos_y * ctx.line_len
					+ ctx.pos_x * ctx.bytespp);
			extract_rgb(ctx.pixel_value, ctx.rgb);
			ctx.texel_index = ctx.pos_y * tex->width + ctx.pos_x;
			ctx.pixels[ctx.texel_index] = (t_vec3f){0.0f, 0.0f,
				rgb8_to_height(ctx.rgb)};
			ctx.pos_x++;
		}
		ctx.pos_y++;
	}
}

typedef struct s_bump_ctx
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
}	t_bump_ctx;

void	bump_bake_dxdy_xy(t_texture *tex)
{
	t_bump_ctx	ctx;

	ctx.width = tex->width;
	ctx.height = tex->height;
	ctx.pixels = (t_vec3f *)tex->pixels;
	ctx.pos_y = -1;
	while (++ctx.pos_y < ctx.height)
	{
		ctx.ym = wrap_index(ctx.pos_y - 1, ctx.height);
		ctx.yp = wrap_index(ctx.pos_y + 1, ctx.height);
		ctx.pos_x = -1;
		while (++ctx.pos_x < ctx.width)
		{
			ctx.xm = wrap_index(ctx.pos_x - 1, ctx.width);
			ctx.xp = wrap_index(ctx.pos_x + 1, ctx.width);
			ctx.dx = (height_at_z(ctx.pixels, ctx.width, ctx.xp, ctx.pos_y)
					- height_at_z(ctx.pixels, ctx.width, ctx.xm, ctx.pos_y))
				* 0.5f;
			ctx.dy = (height_at_z(ctx.pixels, ctx.width, ctx.pos_x, ctx.yp)
					- height_at_z(ctx.pixels, ctx.width, ctx.pos_x, ctx.ym))
				* 0.5f;
			ctx.pixels[ctx.pos_y * ctx.width + ctx.pos_x][0] = ctx.dx;
			ctx.pixels[ctx.pos_y * ctx.width + ctx.pos_x][1] = ctx.dy;
		}
	}
}
