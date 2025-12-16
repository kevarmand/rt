/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan_opt_texture.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 16:20:10 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/16 10:46:52 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include "type.h"
#include "errors.h"
#include "../../parsing_internal.h"
#include <stdint.h>

int	intern_texture_tok(t_texture_parsed *textures, t_tok tok, int *out_id)
{
	char	saved_char;
	void	*found;
	int		new_id;
	int		insert_ret;

	saved_char = tok.start[tok.len];
	tok.start[tok.len] = '\0';
	found = hashmap_get(textures->h_texture, tok.start);
	if (found)
	{
		*out_id = (int)((intptr_t)found) - 1;
		tok.start[tok.len] = saved_char;
		return (SUCCESS);
	}
	new_id = textures->index_t;
	insert_ret = hashmap_insert(textures->h_texture, tok.start,
			(void *)(intptr_t)(new_id + 1));
	tok.start[tok.len] = saved_char;
	if (insert_ret < 0)
		return (ERR_MALLOC);
	textures->index_t++;
	*out_id = new_id;
	return (SUCCESS);
}

int	intern_bumpmap_tok(t_texture_parsed *textures, t_tok tok, int *out_id)
{
	char	saved_char;
	void	*found;
	int		new_id;
	int		insert_ret;

	saved_char = tok.start[tok.len];
	tok.start[tok.len] = '\0';
	found = hashmap_get(textures->h_bumpmap, tok.start);
	if (found)
	{
		*out_id = (int)((intptr_t)found) - 1;
		tok.start[tok.len] = saved_char;
		return (SUCCESS);
	}
	new_id = textures->index_b;
	insert_ret = hashmap_insert(textures->h_bumpmap, tok.start,
			(void *)(intptr_t)(new_id + 1));
	tok.start[tok.len] = saved_char;
	if (insert_ret < 0)
		return (ERR_MALLOC);
	textures->index_b++;
	*out_id = new_id;
	return (SUCCESS);
}

int	scan_opt_texture(t_tok tok, t_element_options *opts,
			t_texture_parsed *textures)
{
	return (intern_texture_tok(textures, tok, &opts->texture_id));
}

int	scan_opt_bump(t_tok tok, t_element_options *opts,
			t_texture_parsed *textures)
{
	return (intern_bumpmap_tok(textures, tok, &opts->bumpmap_id));
}
