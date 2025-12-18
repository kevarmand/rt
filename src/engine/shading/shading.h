/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shading.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 17:10:40 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/18 18:16:55 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHADING_H
# define SHADING_H

# include "vector.h"

//skybox
void	skybox_eval(const t_vec3f direction, const struct s_scene *scene,
			t_vec3f *color_out);
void	sky_color(const t_vec3f direction, t_vec3f *color_out);

//textue sampling
t_vec3f	sample_texture(const t_texture *tex, float u, float v);

#endif