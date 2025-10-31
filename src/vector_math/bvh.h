/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: norivier <norivier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 20:01:56 by norivier          #+#    #+#             */
/*   Updated: 2025/10/31 05:31:21 by norivier         ###   ########.fr       */
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
}	t_primtype;

typedef struct s_mat3x3
{
	float	m[3][3];
}	t_mat3x3;

typedef struct s_triangle
{
	t_vec3f		v0;
	t_vec3f		edge1;
	t_vec3f		edge2;
	t_vec3f		normal;
	t_vec3f		rot;
	t_mat3x3	basis;
	t_mat3x3	inv_basis;
}	t_triangle;

typedef struct s_sphere
{
	t_vec3f	center;
	float	radius;
	float	r_squared;
	float	inv_r;
	t_vec3f	rot;
	t_mat3x3	basis;
	t_mat3x3	inv_basis;
}	t_sphere;

typedef struct s_cylinder
{
	t_vec3f	p0;
	t_vec3f	p1;
	t_vec3f	axis; // (p1 - p0) / height this is normalized
	float	height; // ||p1 - p0||
	float	inv_height;
	float	radius;
	float	r_squared;
	t_vec3f	rot;
	t_mat3x3	basis;
	t_mat3x3	inv_basis;
}	t_cylinder;


typedef struct s_torus
{
	t_vec3f	center;
	t_vec3f	rot;
	float	R;
	float	r;
	float	r_square;
	float	R_square;
	t_mat3x3	basis;
	t_mat3x3	inv_basis;
}	t_torus;

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

typedef struct s_moller
{
	t_vec3f	r_cross_e2;
	float	det;
	float	invdet;
	t_vec3f	s_cross_e1;
	t_vec3f	s;
}	t_moller;

typedef struct s_hit
{
	float	u;
	float	v;
	float	t;
	t_vec3f	inter;
	t_vec3f	normal;
	int		prim_id;
}	t_hit;

typedef struct s_sphere_inter
{
	t_vec3f	L;
	float	b;
	float	c;
	float	t;
	float	t0;
	float	t1;
	float	tmin;
	float	tmax;
	float	disc;
	float	disc_sqrt;
	float	q;
}	t_sphere_inter;

typedef struct s_equ
{
	float	a;
	float	b;
	float	c;
	float	d;
	float	e;
}	t_equ;

// AABB
void	prim_bound(t_primitive *p);
void	prim_nbound(t_primpack pack, t_aabb out);
void	bound_merge(t_aabb a, t_aabb b, t_aabb out);
float	bound_area(t_aabb b);
void	prim_bound_init(t_primpack pack);
int		bound_intersect(t_ray r, t_aabb bound, float *near, float *far);
// Sorting
void	swap_int(int *a, int *b);
void	swap_float(float *a, float *b);
// the 3 following functions are kinda not needed not sure how important is it to sort the roots
void	sortf2(float *a, float *b);
void	sortf3(float *a, float *b, float *c);
void	sortf4(float *a, float *b, float *c, float *d);
// Equations
int		solve_quadratic(t_equ arg, float roots[]);
int		solve_quartic(t_equ arg, float roots[]);
// Inter math
t_vec3f	mat3x3_mulv(t_mat3x3 m, t_vec3f v);
#endif // !BVH_H
