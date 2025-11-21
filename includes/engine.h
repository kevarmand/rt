/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 18:08:04 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/21 21:45:57 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENGINE_H
# define ENGINE_H

#include "vector.h"

typedef struct s_ray {
	t_vec3f	origin;
	t_vec3f	dir;
}	t_ray;

typedef enum e_hit_kind
{
	HIT_NONE,
	HIT_PLANE,
	HIT_PRIMITIVE
}	t_hit_kind;

typedef struct s_hit {
	t_hit_kind	kind;
	float		t;
	t_vec3f		point;
	t_vec3f		normal;
	int			primitive_id;
}	t_hit;


typedef struct s_scene t_scene;

/* ************************************************************************** */
/*								INTERSECTION								  */
/* ************************************************************************** */

int scene_hit(const t_scene *scene, const t_ray *ray, t_hit *out_hit);



/* ************************************************************************** */
/*  							Shading									  	  */
/* ************************************************************************** */


int	shading_ray(t_scene *scene, t_ray *ray, t_vec3f *color_out);

int	shade_hit(t_scene *scene, t_hit *hit, t_vec3f *color_out);


#endif