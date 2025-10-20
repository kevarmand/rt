/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conv_option.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 10:40:46 by kearmand          #+#    #+#             */
/*   Updated: 2025/10/20 15:43:14 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include "io.h"
#include "error_codes.h"
#include <stdlib.h>
#include "libft.h"
#include "scene.h"
#include "convert.h"

typedef struct s_opt_ids
{
	t_index	albedo;
	t_index	normal;
}	t_opt_ids;

static int	resolve_texture_ids(t_conv_ctx *cx, t_element_options *opt,
			t_opt_ids *ids)
{
	if (intern_texture(cx, &opt->texture_path, &ids->albedo) != SUCCESS)
		return (ERR_MALLOC);
	if (intern_texture(cx, &opt->bumpmap_path, &ids->normal) != SUCCESS)
		return (ERR_MALLOC);
	return (SUCCESS);
}

static void	build_material_from_opt(const t_element_options *opt,
			const t_opt_ids *ids, t_material *m)
{
	m->ambient = opt->ambient_occlusion;
	m->diffuse = opt->diffuse_weight;
	m->specular = opt->specular_weight;
	m->shininess = opt->shininess;
	m->reflection = opt->reflection;
	m->refraction = opt->refraction;
	m->ior = opt->ior;
	m->texture_albedo_id = ids->albedo;
	m->texture_normal_id = ids->normal;
}

static int	build_and_intern_material(t_conv_ctx *cx,
			const t_element_options *opt, const t_opt_ids *ids,
			t_index *out_mat)
{
	t_material	m;

	build_material_from_opt(opt, ids, &m);
	return (intern_material(cx, &m, out_mat));
}

static void	build_surface_from_opt(const t_element_options *opt, t_surface *s)
{
	int	i;

	i = 0;
	while (i < 6)
	{
		s->map_uv[i] = opt->uv[i];
		i++;
	}
	s->color.x = 1.0f;
	s->color.y = 1.0f;
	s->color.z = 1.0f;
	s->normal.x = 0.0f;
	s->normal.y = 0.0f;
	s->normal.z = 1.0f;
	mat4_identity(s->w2o);
	mat4_identity(s->o2w);
}

static int	build_and_intern_surface(t_conv_ctx *cx,
			const t_element_options *opt, t_index *out_surf)
{
	t_surface	s;

	build_surface_from_opt(opt, &s);
	return (intern_surface(cx, &s, opt->uv_mod, out_surf));
}
int	intern_texture(t_conv_ctx *cx, char **path, t_index *out_id);

int	conv_option_primitive(t_primitive *prim, t_element_options *opt,
			t_conv_ctx *cx, t_vec3f *color)
{
	t_opt_ids	ids;
	t_index		mat_id;
	t_index		surf_id;

	if (intern_texture(cx, &opt->texture_path, &ids.albedo) != SUCCESS)
		return (ERR_MALLOC);
	if (intern_texture(cx, &opt->bumpmap_path, &ids.normal) != SUCCESS)
		return (ERR_MALLOC);
	if (intern_material(cx, opt, &ids, &mat_id) != SUCCESS)
		return (ERR_MALLOC);
	if (intern_surface(cx, opt, &surf_id, color) != SUCCESS)
		return (ERR_MALLOC);
	prim->material_id = mat_id;
	prim->surface_id = surf_id;
	return (SUCCESS);
}
