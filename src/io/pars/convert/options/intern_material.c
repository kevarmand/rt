/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intern_material.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 13:23:38 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/13 16:22:06 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include "error_codes.h"
#include <stdlib.h>
#include "libft.h"
#include "scene.h"
#include "convert.h"

void	generate_option_key(const t_element_options *opts, char *key, t_opt_ids *ids);
// intern_material(cx, opt, &ids, &mat_id);

static int	material_hit(t_conv_ctx *cx, char *key, t_index *out_mat)
{
	void	*found;

	found = hashmap_get(&cx->mat_m, key);
	if (!found)
		return (0);
	*out_mat = (t_index)((intptr_t)found) - 1;
	return (1);
}

static int	material_build(const t_element_options *m,
			t_opt_ids *ids, t_material *out)
{
	out->ambient = m->ambient_occlusion;
	out->diffuse = m->diffuse_weight;
	out->specular = m->specular_weight;
	out->shininess = m->shininess;
	out->reflection = m->reflection;
	out->refraction = m->refraction;
	out->ior = m->ior;
	out->texture_albedo_id = ids->albedo;
	out->texture_normal_id = ids->normal;
	return (SUCCESS);
}

int	intern_material(t_conv_ctx *cx,
		t_element_options *opt, t_opt_ids *ids, t_index *out_mat)
{
	t_material	new_mat;
	char		key[256];
	int			idx;

	generate_option_key(opt, key, ids);
	if (material_hit(cx, key, out_mat))
		return (SUCCESS);
	if (material_build(opt, ids, &new_mat) != SUCCESS)
		return (ERR_MALLOC);
	idx = vector_push_back(&cx->mat_v, &new_mat);
	if (idx < 0)
		return (ERR_MALLOC);
	if (hashmap_insert(&cx->mat_m, key,
			(void *)(intptr_t)(idx + 1)) < 0)
		return (ERR_MALLOC);
	*out_mat = idx;
	return (SUCCESS);
}
