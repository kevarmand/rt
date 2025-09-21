/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: norivier <norivier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 20:01:56 by norivier          #+#    #+#             */
/*   Updated: 2025/09/14 21:51:12 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BVH_H
# define BVH_H 1

# include "vector.h"
# define MALLOC_ERR -2
# define LEAF_THRESHOLD 4

typedef enum e_primtype
{
	PRIM_TRIANGLE,
	PRIM_SPHERE,
	PRIM_CYLINDER,
	PRIM_TORUS,
	PRIM_QUAD
}	t_primtype;

typedef struct s_triangle
{
	t_vec3f		v0;
	t_vec3f		edge1;
	t_vec3f		edge2;
}	t_triangle;

typedef struct s_sphere
{
	t_vec3f	center;
	float	radius;
}	t_sphere;

typedef struct s_cylinder
{
	t_vec3f	p0;
	t_vec3f	p1;
	float	radius;
}	t_cylinder;

typedef struct s_torus
{
	t_vec3f	center;
	float	R;
	float	r;
}	t_torus;

typedef struct s_quad
{
	t_vec3f	v0;
	t_vec3f	v1;
	t_vec3f	v2;
	t_vec3f	v3;
}	t_quad;

typedef t_vec3f t_aabb[2];

typedef struct s_primitive
{
	t_primtype		type;
	union
	{
		t_triangle	tr;
		t_sphere	sp;
		t_cylinder	cy;
		t_torus		to;
		t_quad		qu;
	};
	t_aabb			bounds;
	t_vec3f			centroid;
}	t_primitive;

typedef struct s_ray
{
	t_vec3f	origin;
	t_vec3f	dir;
	t_vec3f	invdir;
	t_vec3f	orig_div;
	int		sign[3];
}	t_ray;

typedef struct s_hit
{
	int		hit;
	float	t;
	int		prim_id;
}	t_hit;

typedef struct s_bvhnode
{
	t_aabb	bounds;
	int		start;
	int		count;
	int		left;
	int		right; // indices of child nodes (-1 if leaf)
	int		parent; // -1 for root
}	t_bvhnode;

typedef struct s_primpack
{
	t_primitive	*p;
	int			*indice;
	int			start;
	int			count;
}	t_primpack;

// fuck the norme, amirite ?
typedef struct s_aabb_inter
{
	float	t1x;
	float	t2x;
	float	tminx;
	float	tmaxx;
	float	t1y;
	float	t2y;
	float	tminy;
	float	tmaxy;
	float	t1z;
	float	t2z;
	float	tminz;
	float	tmaxz;
}	t_aabb_inter;

#endif // !BVH_H
