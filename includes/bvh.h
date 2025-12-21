/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 20:01:56 by norivier          #+#    #+#             */
/*   Updated: 2025/12/21 02:10:58 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <complex.h>
#include <stdint.h>
#ifndef BVH_H
# define BVH_H 1

# include <stdint.h>
# include <stddef.h>
# include "vector.h"
# include "scene.h"
# include "engine.h"
# define MALLOC_ERR -2
# define LEAF_THRESHOLD 4
# define STACK_MAX_DEPTH 64
# define BIN_COUNT 16

typedef struct s_transform
{
	t_mat3x3f		rot;
	t_vec3f			scale;
	t_vec3f			trans;
}	t_transform;

typedef struct s_bin
{
	t_aabb	bounds;
	int		count;
}	t_bin;

typedef struct s_primref
{
	t_aabb		bounds;
	t_vec3f		centroid;
	uint32_t	prim_id;
}	t_primref;

typedef struct s_bvh_child
{
	int			start;
	int			count;
	int			node_idx;
}	t_bvh_child;

typedef struct s_bvh_buf
{
	t_primref	*pref;
	int			*pref_idx;
	t_bvh_child	stack[STACK_MAX_DEPTH];
	int			sp;
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

typedef struct s_cylinder_inter_ctx
{
	float	ro_dot;
	float	rd_dot;
	float	roots[2];
	int		nroots;
}	t_cylinder_inter_ctx;

typedef struct s_equ
{
	float	a;
	float	b;
	float	c;
	float	d;
	float	e;
}	t_equ;

typedef struct s_torus_interf_ctx
{
	t_vec3f	ux;
	t_vec3f	uy;
	t_vec3f	uz;
	t_equ	eq;
	float	roots[4];
	int		nroots;
}	t_torus_interf_ctx;

// AABB
t_aabb	prim_bound(t_primitive *p);
t_aabb	bound_merge(t_aabb a, t_aabb b);
float	bound_area(t_aabb b);
void	prim_bound_init(t_bvh_buf *buf, t_primitive *prims, int count);
t_aabb	prim_bound_range(t_primref *pref, int *indice, int start, int count);
int		bound_intersect(t_ray r, t_aabb bound, float *near, float *far);
// Sorting
void	swap_int(int *a, int *b);
void	swap_float(float *a, float *b);
// the 3 following functions are kinda not needed not sure how important is it to sort the roots
void	sortf2(float *a, float *b);
void	sortf3(float *a, float *b, float *c);
void	sortf4(float *a, float *b, float *c, float *d);
// Equations
int		solve_quad(t_equ arg, float roots[]);
int		solve_quarticf(t_equ arg, float roots[]);
int		filter_real_numbers(int numvalues, complex double in[], double out[]);
// Inter math
t_vec3f	mat3x3_mulv(t_mat3x3f m, t_vec3f v);
// SAH
float	prim_centroid_axis(const t_primref *p, int axis);
void	zero_bins(t_bin *bins, size_t siz);
int		fill_bins(t_bvh_buf *b,t_bvh_child e, t_vec3f *cbounds, int axis);
void	instance_cbounds(t_bvh_buf *b, t_bvh_child e, t_vec3f *cbounds);
int		sah_split(t_bvh_child e, t_bvh_buf *b);
// Primitive Inter functions
int	triangle_inter(t_ray r, t_triangle *tr, t_hit *hit);
int	sphere_inter(t_ray r, t_sphere *s, t_hit *hit);
int	cylinder_inter(t_ray r, t_cylinder *cl, t_hit *hit);
int	torus_inter(t_ray r, t_torus *t, t_hit *hit);
int	torus_inter_shadowf(t_ray r, t_torus *t, t_hit *hit);
int	prim_inter(t_ray r, t_primitive *p, t_hit *out, float tnear);
int	torus_interf(t_ray r, t_torus *t, t_hit *hit);
void	build_bvh(t_bvhnode *nodes, t_bvh_buf *buf, int primcount);
t_primitive	*reorder_prims(t_primitive *prims, int *idx, size_t count);
int	bvh_inter(t_ray r, t_bvhnode *nodes, t_primitive *prims, t_hit *out);
// Centroid
t_vec3f	prim_centroid(t_primitive *p);
#endif // !BVH_H
