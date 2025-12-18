/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xpm_albedo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 17:57:28 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/17 18:45:21 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "xpm.h"

typedef struct s_albedo_ctx
{
	int		pos_y;
	int		pos_x;
	int		rgb[3];
	int		pixel_value;
	int		texel_index;
	int		line_len;
	int		bytespp;
	t_vec3f	*pixels;
}	t_albedo_ctx;

void	copy_albedo_from_mlx(t_texture *tex, char *addr,
				int bpp_line[2])
{
	t_albedo_ctx	ctx;

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
			rgb8_to_linear_vec(ctx.rgb, &ctx.pixels[ctx.texel_index]);
			ctx.pos_x++;
		}
		ctx.pos_y++;
	}
}
