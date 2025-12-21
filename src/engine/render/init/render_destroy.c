/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_destroy.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 01:56:09 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 02:42:22 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "render.h"
#include "engine.h"
#include "bitmap.h"
#include "display.h"
#include "mlx.h"
#include "scene.h"

void	destroy_render(t_render *render)
{
	int	i;
	
	i = 0;
	while (i < render->workers_count)
	{
		free(render->workers.array[i].tile.hdr_pixels);
		i++;
	}
	free(render->workers.array);
	free(render->manager.rgb_buffer);
	free(render->manager.hdr_buffer);
	free(render->manager.tonemap_exposure);
	bitmap_destroy(&render->manager.tileset.tile_state);
	free(render->mailbox.rgb_pixels);
}

void	destroy_display(t_display *display)
{
	int	i;

	if (!display)
		return ;
	i = 0;
	while (i < display->total_cams)
	{
		free(display->frame[i].rgb_pixels);
		i++;
	}
	free(display->frame);
	if (!display->mlx)
		return ;
	mlx_destroy_image(display->mlx, display->ui.img.img_ptr);
	mlx_destroy_image(display->mlx, display->main_img.img_ptr);
	mlx_destroy_window(display->mlx, display->win);
	mlx_destroy_display(display->mlx);
	free(display->mlx);
}

void	destroy_texture(t_texture *texture,int size)
{
	int	i;
	
	if (!texture)
		return ;
	
	i = 0;
	while (i < size)
	{
		free(texture[i].path);
		free(texture[i].pixels);
		i++;
	}
	free(texture);
}

void destroy_scene(t_scene *scene)
{
	if (!scene)
		return ;
	free(scene->primitives);
	free(scene->bvh_nodes);
	free(scene->planes);
	free(scene->materials);
	free(scene->surfaces);
	destroy_texture(scene->textures, scene->texture_count);
	destroy_texture(scene->bumpmaps, scene->bumpmap_count);
	free(scene->lights);
	free(scene->cameras);
}
