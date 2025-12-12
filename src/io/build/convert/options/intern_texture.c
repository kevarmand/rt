/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intern_texture.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 13:24:01 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/12 18:13:57 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scene.h"
#include "../convert.h"
#include "io.h"
#include "errors.h"
#include <stdlib.h>
#include <stdint.h>
#include "libft.h"

void	cleanup_str(char **s)
{
	free(s[0]);
	s[0] = NULL;
}

static void	init_default_texture(t_texture *tex)
{
	tex->path = NULL;
	tex->width = 0;
	tex->height = 0;
	tex->channels = 0;
	tex->pixels = NULL;
}

static int	texture_hit(t_conv_ctx *cx, char **path, t_index *out_id)
{
	void	*found;

	found = hashmap_get(cx->tex_m, *path);
	if (!found)
		return (0);
	cleanup_str(path);
	*out_id = (t_index)((intptr_t)found) - 1;
	return (1);
}

static int	texture_emplace(t_conv_ctx *cx, char **path, t_index *out_id)
{
	t_texture	new_tex;
	int			index;

	init_default_texture(&new_tex);
	new_tex.path = *path;
	if (vector_push_back(&cx->tex_v, &new_tex) != SUCCESS)
		return (ERR_MALLOC);
	index = vector_size(&cx->tex_v) - 1;
	if (index < 0)
		return (ERR_MALLOC);
	*path = NULL;
	if (hashmap_insert(cx->tex_m, new_tex.path,
			(void *)(intptr_t)(index + 1)) < 0)
		return (ERR_MALLOC);
	*out_id = index;
	return (SUCCESS);
}

#include <stdio.h>
int	intern_texture(t_conv_ctx *cx, char **path, t_index *out_id)
{
	if (!*path)
	{
		*out_id = SCENE_ID_NONE;
		return (SUCCESS);
	}
	if (texture_hit(cx, path, out_id))
		return (SUCCESS);
	return (texture_emplace(cx, path, out_id));
}
