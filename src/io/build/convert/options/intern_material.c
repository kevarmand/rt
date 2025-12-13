/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intern_material.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 13:23:38 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/13 16:25:24 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include "errors.h"
#include <stdlib.h>
#include "libft.h"
#include "scene.h"
#include "../convert.h"
#include <stdint.h>

static int	material_hit(t_conv_ctx *cx, char *key, t_index *out_mat)
{
	void	*found;

	found = hashmap_get(cx->mat_m, key);
	if (!found)
		return (0);
	*out_mat = (t_index)((intptr_t)found) - 1;
	return (1);
}

static void	material_build(const t_element_options *m, t_material *out)
{
	out->ambient = m->ambient_occlusion;
	out->diffuse = m->diffuse_weight;
	out->specular = m->specular_weight;
	out->shininess = m->shininess;
	out->reflection = m->reflection;
	out->refraction = m->refraction;
	out->ior = m->ior;
	out->texture_albedo_id = m->texture_id;
	out->texture_normal_id = m->bumpmap_id;
}

int	intern_material(t_conv_ctx *cx,
		t_element_options *opt, t_index *out_mat)
{
	t_material	new_mat;
	char		key[256];
	int			idx;
	
	material_build(opt, &new_mat);
	generate_option_key(&new_mat, key);
	if (material_hit(cx, key, out_mat))
		return (SUCCESS);
	idx = vector_push_back(&cx->mat_v, &new_mat);
	if (idx < 0)
		return (ERR_MALLOC);
	idx = vector_size(&cx->mat_v) - 1;
	if (hashmap_insert(cx->mat_m, key,
			(void *)(intptr_t)(idx + 1)) < 0)
		return (ERR_MALLOC);
	*out_mat = idx;
	return (SUCCESS);
}
