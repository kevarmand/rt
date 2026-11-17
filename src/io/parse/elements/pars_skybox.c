/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_skybox.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 23:07:05 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/18 21:31:22 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include "errors.h"
#include "../parsing_internal.h"

static int	tok_eq_lit(t_tok tok, const char *lit)
{
	int	index;

	index = 0;
	while (lit[index] && index < tok.len)
	{
		if (tok.start[index] != lit[index])
			return (0);
		index++;
	}
	if (index != tok.len)
		return (0);
	return (lit[index] == '\0');
}

static int	parse_skybox_texture(t_scene_parsed *scene, t_tok tok, int *out_id)
{
	if (tok_eq_lit(tok, "none"))
	{
		*out_id = -1;
		return (SUCCESS);
	}
	return (intern_texture_tok(&scene->textures, tok, out_id));
}

static int	parse_skybox_uv(t_scene_parsed *scene, t_tok tok)
{
	t_element_options	opts;
	t_tok				value;

	if (tok.len <= 3 || tok.start[0] != 'u'
		|| tok.start[1] != 'v' || tok.start[2] != '=')
		return (ERR_PARS);
	value.start = tok.start + 3;
	value.len = tok.len - 3;
	if (scan_opt_uv(value, &opts, &scene->textures) != SUCCESS
		|| opts.uv_mod != 2)
		return (ERR_PARS);
	ft_memcpy(scene->skybox.uv, opts.uv, sizeof(scene->skybox.uv));
	scene->skybox.uv_enabled = 1;
	return (SUCCESS);
}

static int	parse_skybox_intensity(t_pars_state *st,
		t_scene_parsed *scene, t_tok token)
{
	if (scan_vec3(token, scene->skybox.intensity) != SUCCESS
		|| scene->skybox.intensity[0] < 0.0f
		|| scene->skybox.intensity[1] < 0.0f
		|| scene->skybox.intensity[2] < 0.0f)
		return (ERR_PARS);
	if (!pars_next_tok(st, &token))
		return (SUCCESS);
	if (parse_skybox_uv(scene, token) != SUCCESS)
		return (ERR_PARS);
	if (pars_next_tok(st, &token))
		return (ERR_PARS);
	return (SUCCESS);
}

int	pars_skybox(t_pars_state *st, t_scene_parsed *scene)
{
	t_tok	token;

	if (!pars_next_tok(st, &token)
		|| parse_skybox_texture(scene, token, &scene->skybox.texture_id)
		!= SUCCESS)
		return (ERR_PARS);
	if (!pars_next_tok(st, &token))
		return (SUCCESS);
	if (tok_eq_lit(token, "sp") || tok_eq_lit(token, "cy"))
	{
		scene->skybox.mode = SKYBOX_SPHERE;
		if (tok_eq_lit(token, "cy"))
			scene->skybox.mode = SKYBOX_CYLINDER;
		if (!pars_next_tok(st, &token))
			return (SUCCESS);
	}
	return (parse_skybox_intensity(st, scene, token));
}
