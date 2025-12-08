/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: norivier <norivier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 20:01:56 by norivier          #+#    #+#             */
/*   Updated: 2025/11/13 22:15:52 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#ifndef BVH_H
# define BVH_H 1

# include "vector.h"
# define MALLOC_ERR -2
# define LEAF_THRESHOLD 4
# define BIN_COUNT 16

typedef enum __attribute__((packed)) e_primtype : uint8_t
{
	PRIM_TRIANGLE,
	PRIM_SPHERE,
	PRIM_CYLINDER,
	PRIM_TORUS,
}	t_primtype;

typedef struct s_triangle
{
	t_vec3f		v0;
	t_vec3f		edge1;
	t_vec3f		edge2;
	t_vec3f		normal;
}	t_triangle;

typedef struct s_sphere
{
	t_vec3f	center;
	float	radius;
	float	r_squared;
	float	inv_r;
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
}	t_cylinder;

typedef struct s_torus
{
	t_vec3f	center;
	float	R;
	float	r;
	float	r_square;
	float	R_square;
}	t_torus;

typedef struct s_transform
{
	t_mat3x3f		rot;
	t_vec3f			scale;
	t_vec3f			trans;
}	t_transform;

typedef struct s_aabb
{
	t_vec3f	b[2];
}	t_aabb;

typedef struct __attribute__((aligned(16))) s_primitive
{
	t_primtype		type;
	union
	{
		t_triangle	tr;
		t_sphere	sp;
		t_cylinder	cy;
		t_torus		to;
	};
}	t_primitive;

typedef struct s_primref
{
	t_aabb		bounds;
	t_vec3f		centroid;
	uint32_t	prim_id;
}	t_primref;

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
	uint8_t	is_leaf;
	union
	{
		struct s_node
		{
			uint32_t	left;
			uint32_t	right;
		}	node;
		struct s_leaf
		{
			uint32_t	start;
			uint32_t	count;
		}	leaf;
	};
}	t_bvhnode;

typedef struct s_blas
{
	t_primitive	*p;
	uint32_t	prim_count;
	t_primref	*pref;
	uint32_t	primref_count;
	t_bvhnode	*nodes;
	uint32_t	node_count;
}	t_blas;

typedef struct s_object
{
	t_blas		*blas;
	t_mat3x4f	o_t_w;
	t_mat3x4f	w_t_o;
	t_aabb		bounds;
}	t_object;

typedef t_bvhnode	t_tlas_node;

typedef struct s_tlas
{
	t_object	*objects;
	uint32_t	obj_count;
	t_tlas_node	*nodes;
	uint32_t	node_count;
}	t_tlas;

typedef struct s_bvh_child
{
	int			start;
	int			count;
	int			node_idx;
}	t_bvh_child;

typedef struct s_bin
{
	t_aabb	bounds;
	int		count;
}	t_bin;

typedef struct s_bvh_buf
{
	t_primref	*pref;
	int			*pref_idx;
	t_bin		bins[BIN_COUNT];
	t_aabb		prefix[BIN_COUNT];
	t_aabb		suffix[BIN_COUNT];
	int			left_counts[BIN_COUNT];
	float		bestcost;
	int			bestaxis;
	int			bin_split_idx;
	int			nodecount;
}	t_bvh_buf;


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
t_aabb	prim_bound(t_primitive *p);
void	prim_nbound(t_bvh_child pack, t_aabb out);
t_aabb	bound_merge(t_aabb a, t_aabb b);
float	bound_area(t_aabb b);
void	prim_bound_init(t_primref *pref, t_primitive *prims, int count);
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
t_vec3f	mat3x3_mulv(t_mat3x3f m, t_vec3f v);
#endif // !BVH_H
