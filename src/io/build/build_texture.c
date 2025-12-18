/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_texture.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 16:34:12 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/18 21:29:34 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "errors.h"
#include "scene.h"
#include <stdint.h>
#include "libft.h"
#include "convert/convert.h"
#include "io.h"

static void	texture_init_one(t_texture *tex, char *path)
{
	tex->path = path;
	tex->width = 0;
	tex->height = 0;
	tex->channels = 0;
	tex->pixels = NULL;
}

static int	texture_detach_one(t_texture *dst, t_hashnode *node)
{
	int	id;

	id = (int)((intptr_t)node->value) - 1;
	texture_init_one(&dst[id], node->key);
	node->key = NULL;
	node->value = NULL;
	return (SUCCESS);
}

static int	textures_detach_all(t_texture *dst, t_hashmap *map)
{
	size_t	index;

	index = 0;
	while (index < map->capacity)
	{
		if (map->nodes[index].key && map->nodes[index].value)
			texture_detach_one(dst, &map->nodes[index]);
		index++;
	}
	return (SUCCESS);
}

int	build_textures(t_scene *scene, t_texture_parsed *parsed)
{
	int	count;

	count = parsed->index_t;
	scene->texture_count = count;
	if (count == 0)
		scene->textures = NULL;
	else
	{
		scene->textures = ft_calloc((size_t)count, sizeof(t_texture));
		if (!scene->textures)
			return (ERR_MALLOC);
		textures_detach_all(scene->textures, parsed->h_texture);
	}
	count = parsed->index_b;
	scene->bumpmap_count = count;
	if (count == 0)
		scene->bumpmaps = NULL;
	else
	{
		scene->bumpmaps = ft_calloc((size_t)count, sizeof(t_texture));
		if (!scene->bumpmaps)
			return (ERR_MALLOC);
		textures_detach_all(scene->bumpmaps, parsed->h_bumpmap);
	}
	return (SUCCESS);
}

void	parsed_textures_destroy(t_texture_parsed *parsed)
{
	hashmap_destroy(parsed->h_texture, NULL);
	hashmap_destroy(parsed->h_bumpmap, NULL);
	parsed->h_texture = NULL;
	parsed->h_bumpmap = NULL;
	parsed->index_t = 0;
	parsed->index_b = 0;
}
