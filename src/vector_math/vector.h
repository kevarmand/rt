/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: norivier <norivier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 16:47:11 by norivier          #+#    #+#             */
/*   Updated: 2025/10/01 21:33:11 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_H
# define VECTOR_H 1

typedef struct s_vec3f
{
	float	x;
	float	y;
	float	z;
}	t_vec3f;

t_vec3f	vec3f_min(t_vec3f a, t_vec3f b);
t_vec3f	vec3f_max(t_vec3f a, t_vec3f b);
t_vec3f	vec3f_cross(t_vec3f a, t_vec3f b);
float	vec3f_dot(t_vec3f a, t_vec3f b);
t_vec3f	vec3f_scale(t_vec3f a, float s);
t_vec3f	vec3f_add(t_vec3f a, t_vec3f b);
t_vec3f	vec3f_sub(t_vec3f a, t_vec3f b);
t_vec3f	vec3f_normalize(t_vec3f a);

#endif // !VECTOR_H
