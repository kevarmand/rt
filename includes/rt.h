/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmand <karmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/10 13:22:08 by karmand           #+#    #+#             */
/*   Updated: 2020/03/05 23:29:16 by karmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_H
# define RT_H

# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <math.h>
# include "mlx.h"
# include "keycode.h"

# include <X11/X.h>

# define SUCCESS			0
# define ERROR				-1

# define ERR_ARG			-2
# define ERR_EXT			-3
# define ERR_OPEN			-4
# define ERR_MIN			-5
# define ERR_MALLOC			-100

# define ERR_MLX			-6
# define ERR_BMP			-7

# define ERR_DOUBLE			-200
# define ERR_COLOR			-201

# define ERR_A				-210
# define ERR_R				-211
# define ERR_C				-212
# define ERR_L				-213
# define ERR_SP				-214
# define ERR_PL				-215
# define ERR_TR				-216
# define ERR_CY				-217
# define ERR_SQ				-218
# define ERR_R_REDEF		-219
# define ERR_A_REDEF		-220
# define ERR_PARS_TYPE		-221

# define VITESSE_TRANSLAT	3
# define VITESSE_ROTATION	0.3
# define VITESSE_RESIZE		1.25
# define SIN_VR				0.34202014332
# define COS_VR				0.93969262078
# define AXE_XP				1
# define AXE_XM				2
# define AXE_YP				3
# define AXE_YM				4
# define AXE_ZP				5
# define AXE_ZM				6
# define PLUS				1
# define MOINS				0

# define ERR_EMPTY_OBJ		-16

# define ERR_NORM			-300

typedef struct		s_color
{
	int				r;
	int				g;
	int				b;
}					t_color;

typedef struct		s_point
{
	double			x;
	double			y;
	double			z;
}					t_point;

typedef struct		s_vector
{
	double			x;
	double			y;
	double			z;
}					t_vector;

/*
**	Liste des objet
*/

typedef enum		e_type
{
	SPHERE,
	PLANE,
	CYLINDER,
	TRIANGLE,
	SQUARE,
	CAM,
	LIGHT
}					t_type;

typedef struct		s_sp
{
	t_point			centre;
	t_color			color;
	double			rayon;
}					t_sp;

typedef struct		s_pl
{
	t_point			point;
	t_vector		norm;
	t_color			color;
	double			a;
	double			b;
	double			c;
	double			d;
}					t_pl;

typedef struct		s_cy
{
	t_point			point;
	t_vector		dir;
	double			rayon;
	double			height;
	t_color			color;
}					t_cy;

typedef struct		s_tr
{
	t_point			p1;
	t_point			p2;
	t_point			p3;
	double			a;
	double			b;
	double			c;
	double			d;
	t_vector		norm;
	t_color			color;
}					t_tr;

typedef struct		s_sq
{
	t_point			point;
	t_vector		norm;
	double			height;
	t_color			color;
	t_vector		dir1;
	t_vector		dir2;
	double			a;
	double			b;
	double			c;
	double			d;
}					t_sq;

typedef struct		s_cam
{
	t_point			point;
	t_vector		dir;
	double			fov;
	t_vector		hori;
	t_vector		vert;
	t_point			first;
}					t_cam;

typedef struct		s_ray
{
	t_point			source;
	t_vector		dir;
	int				inter;
	double			dist;
}					t_ray;

typedef struct		s_lighting
{
	t_point			inter;
	t_vector		norm;
	t_color			c_obj;
	t_color			c_ret;
}					t_lighting;

typedef struct		s_light
{
	t_point			point;
	double			brightness;
	t_color			color;
}					t_light;

/*
**	fin des list objet
*/

typedef struct		s_list
{
	void			*obj;
	t_type			type;
	struct s_list	*next;
}					t_list;

typedef struct		s_para
{
	int				res_width;
	int				res_height;
	double			brightness;
	t_color			color;
}					t_para;

typedef struct		s_plot
{
	void			*p;
	t_type			type;
	double			dist;
}					t_plot;

typedef struct		s_ldist
{
	int				n;
	t_plot			*plot;
}					t_ldist;

typedef struct		s_image
{
	void			*img_ptr;
	void			*addr_ptr;
	int				bpp;
	int				size_l;
	int				endiant;
	int				width;
	int				height;
}					t_image;

typedef struct		s_select
{
	t_list			*obj;
	t_list			*light;
	t_list			*cam;
}					t_select;

/*
**	pour les test
**	test[0] echap
**	test[1]	resolution
**	test[2]	ambiance
*/

typedef struct		s_data
{
	t_para			para;
	t_list			*lobj;
	t_list			*llight;
	t_list			*lcam;
	t_select		select;
	t_ldist			ldist;
	void			*mlx_ptr;
	void			*win_ptr;
	t_image			*view;
	int				test[3];
}					t_data;

/*
**	Fonction de liste chainee
*/

int					list_addback(t_list **l, void *p);
int					list_addfront(t_list **l, void *p, t_type t);
int					list_count(t_list *l);
int					list_free(t_list *l);

/*
**		sructure pour intersection.
*/

int					ft_exit(t_data *data);
int					ft_data_exit(t_data *data);
int					ft_exit_bmp(t_data *data);
int					data_dest(t_data *data);
void				data_init(t_data *data);
int					parsing(int fd, t_data *data);
int					error(int code);
int					err_op(int code, char *str);
int					err2(int code, char *str, int n);

/*
**		Fonction de lib
*/

void				ft_bzero(void *s, int n);
double				ft_abs(double a);
int					ft_strcmp(char *s1, char *s2);
int					ft_isdigit(char c);
int					ft_iswp(char c, char *wp);
int					ft_strlen(char *str);
int					get_next_line(int fd, char **line);
char				**ft_split(char *str, char *wp);
char				*ft_strjoin(char **arr, char *sep);
int					free_split(char **arr);
int					ft_free(void *ptr);
int					split_count(char **arg);

/*
**		Fonction de parsing
*/

int					parsing(int fd, t_data *data);
int					get_int(char *str, int *n);
int					get_double(char *str, double *d);
int					pars_color(char *str, t_color *color);
int					pars_point(char *str, t_point *point);
int					pars_vector(char *str, t_vector *v);

/*
**		fonction d'image
*/

int					bmp(t_data *data, char *name);

/*
**	Fonction de rendu
*/

int					render(t_data *data);

/*
**	Fonction de distance
*/

int					dist_init(t_data *d, t_ldist *p);
int					dist_destroy(t_ldist *p);

/*
**	fonction de cam
*/

int					cam_gen(t_data *data, t_cam *c);

/*
**	FONCTIN DE CHANGEEMENT
*/

t_vector			translate_type(int type);
int					translate_plane(t_pl *pl, int type);
int					translate_sphere(t_sp *sp, int type);
int					translate_square(t_sq *sq, int type);
int					translate_triangle(t_tr *tr, int type);
int					translate_cylinder(t_cy *cy, int type);
int					translate_cam(t_cam *cam, int type);
int					translate_switch(int type, t_data *data);
int					translate_cam(t_cam *cam, int type);
int					translate_light(t_light *light, int type);

int					rot_vect(t_vector *v, int type);
int					rotate_switch(int type, t_data *data);
int					rotate_cam(t_cam *cam, int type);

int					resize_switch1(int type, t_data *data);
int					resize_switch2(int type, t_data *data);

int					select_init(t_data *data);
int					select_next_obj(t_data *data);
int					select_next_light(t_data *data);
int					select_next_cam(t_data *data);

/*
**			Fonction de vector
*/

void				vect_set(t_vector *v, int x, int y, int z);
double				vect_norm(t_vector *v);
void				vect_tonorm(t_vector *v);
t_vector			vect_vect(t_vector *u, t_vector *v);
double				vect_dot(t_vector *u, t_vector *v);

t_point				vect_translate(t_point *p, t_vector *v);
t_vector			vect_vector(t_point *p1, t_point *p2);
void				vect_add(t_vector *v1, t_vector *v2);
void				vect_sub(t_vector *v1, t_vector *v2);
int					vect_is_col(t_vector *v1, t_vector *v2);
t_vector			vect_def(double x, double y, double z);
t_vector			vect_mult(t_vector *v, double x);
t_point				vect_set_p(double x, double y, double z);

/*
**			parse_object
*/

int					pars_a(char **arr, t_data *data);
int					pars_res(char **arr, t_data *data);
int					pars_cam(char **arr, t_data *data);
int					pars_light(char **arr, t_data *data);
int					pars_sp(char **arr, t_data *data);
int					pars_tr(char **arr, t_data *data);
int					pars_pl(char **ar, t_data *data);
int					pars_cy(char **ar, t_data *data);
int					pars_sq(char **ar, t_data *data);

/*
**			Fonction de tranformation/Verification
*/

int					init_cam(t_cam *cam, t_data *data);
int					init_light(t_light *l);
int					init_sp(t_sp *sp);
int					init_pl(t_pl *pl);
int					init_tr(t_tr *tr);
int					init_cy(t_cy *cy);
int					init_sq(t_sq *sq);

int					norm_pl(t_pl *pl);
int					norm_tr(t_tr *tr);
int					norm_sq(t_sq *sq);

/*
**			Fonction de resolution d'equation
*/

int					m_pow(int a, int b);
double				equa_premier(double a, double b);
double				equa_second(double a, double b, double c);

/*
**			Fonction de matrice
*/

void				mat_prod(double mr[3][3], double m1[3][3], double m2[3][3]);
void				mat_prod_vect(t_vector *v, double m[3][3], t_vector *u);
void				mat_gen_pass(double m[3][3], t_vector *v1, t_vector *v2);
void				mat_transpose(double r[3][3], double m[3][3]);

/*
**			Fonction d intersection
*/
double				inter_triangle(t_point *p, t_vector *v, t_tr *tr);
double				inter_cylinder(t_point *p, t_vector *v, t_cy *cy);
int					inter_inter(t_ray *r, t_ldist *l, int i);

/*
**			Fonction color
*/

t_color				color_switch(t_data *data, t_ray *r);

/*
**			Fonction light
*/

int					light_sp(t_lighting *li, t_sp *sp, t_point *p);
int					light_sq(t_lighting *li, t_sq *sq);
int					light_tr(t_lighting *li, t_tr *tr);
int					light_pl(t_lighting *li, t_pl *pl);
int					light_cy(t_lighting *li, t_cy *cy, t_point *p);
int					light_add(t_lighting *li, t_color *color, double a);
int					light_init(t_lighting *li, t_plot *plot,
						t_color *color, double b);

int					ft_init_view(t_data *data);
int					ft_init_win(t_data *data, char *name);
int					ft_init_bmp(t_data *data, char *name);
int					key_hook(int keycode, t_data *data);

#endif
