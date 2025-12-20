/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shading.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 17:10:40 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/20 00:07:38 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHADING_H
# define SHADING_H

# include "vector.h"
# include "scene.h"
# include "engine.h"

//skybox
void	skybox_eval(const t_vec3f direction, const struct s_scene *scene,
			t_vec3f *color_out);
void	sky_color(const t_vec3f direction, t_vec3f *color_out);

//textue sampling
t_vec3f	sample_texture(const t_texture *tex, float u, float v);

float	fresnel_schlick(const t_material *mat,
			const t_hit *hit, const t_shading_ctx *ctx);

#endif