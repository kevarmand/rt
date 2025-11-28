/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 16:47:11 by norivier          #+#    #+#             */
/*   Updated: 2025/11/27 21:26:15 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_H
# define VECTOR_H 1

# if defined(__clang__)
typedef float t_vec4f __attribute__((ext_vector_type(4)));
# elif defined (__GNUC__)
typedef float t_vec4f __attribute__((vector_size(sizeof(float) * 4)));
# endif

typedef t_vec4f t_vec3f;

t_vec3f	vec3f_min(t_vec3f a, t_vec3f b);
t_vec3f	vec3f_max(t_vec3f a, t_vec3f b);
t_vec3f	vec3f_cross(t_vec3f a, t_vec3f b);
float	vec3f_dot(t_vec3f a, t_vec3f b);
t_vec3f	vec3f_scale(t_vec3f a, float s);
t_vec3f	vec3f_add(t_vec3f a, t_vec3f b);
t_vec3f	vec3f_sub(t_vec3f a, t_vec3f b);
t_vec3f	vec3f_normalize(t_vec3f a);
void	vec3f_load3(t_vec3f *dst, const float src[3]);
float	vec3f_length(t_vec3f a);
t_vec3f vec3f_mul(t_vec3f a, t_vec3f b);

#endif // !VECTOR_H