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

# define SUCCESS			0
# define ERROR				-1
# define ERR_MALLOC			-100

# define ERR_DOUBLE			-200
# define ERR_COLOR			-201

# define ERR_A				-210
# define ERR_R				-211
# define ERR_C				-212
# define ERR_L				-213
# define ERR_SP				-214
# define ERR_PL				-215
# define ERR_TR				-216

# define ERROR_OPEN			-1
# define ERROR_ARGUMENT		-2
# define ERROR_MALLOC		-3
# define ERROR_PARS_COLOR	-4
# define ERROR_PARS_POINT	-5
# define ERROR_PARS_TYPE	-6
# define ERROR_DEF_RES		-7
# define ERROR_CAM_MISS		-8
# define ERROR_READ			-9
# define ERROR_FOV			-10
# define ERROR_             -11
# define ERROR_MLC			-12
# define ERROR_PARS_VECTOR	-13
# define ERROR_BRIGHTNESS	-14
# define ERROR_RAYON		-15
# define ERROR_EMPTY_OBJ	-16
# define ERROR_PLANE_NORM	-17
# define ERROR_TR_FLAT		-18
# define ERROR_INT			-19

// rgb(135,206,235)

typedef struct	s_buffer
{
	char	*buff;
	int		size;
	int		cur;
}				t_buffer;

typedef struct		s_color
{
	int				r;
	int				g;
	int			 	b;
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
	CILYNDER,
	TRIANGLE,
	SQUARE,
	CAM,
	LIGHT,
	CIRCLE
}					t_type;

typedef struct		s_sp
{
	t_point			centre;
	t_color			color;
	double			rayon;
}					t_sp;
typedef struct		s_ci
{
	t_point			centre;
	t_color			color;
	double			rayon;
	t_vector		norm;
	double			a;
	double			b;
	double			c;
	double			d;
}					t_ci;

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
	t_vector		norm;
	int 			rayon;
	int				height;
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

typedef struct		s_data
{
	t_para			para;
	t_list			*lobj;
	t_list			*llight;
	t_list			*lcam;
	t_ldist			ldist;
	void			*mlx_ptr;
	void			*win_ptr;
	t_image			*view;
	
}					t_data;

/*
**	Fonction de liste chainee
**/

int					list_addback(t_list **l, void *p);
int					list_addfront(t_list **l, void *p, t_type t);
int					list_count(t_list *l);

//sructure pour intersection.

int					data_dest(t_data *data);
void				data_init(t_data *data);
int					parsing(int fd, t_data *data);
int 				error(int code);
int					err1(int code, char *str);

//fonction de lib

int					ft_strcmp(char *s1, char *s2);
int					ft_isdigit(char c);
int					ft_iswp(char c, char *wp);
int					ft_strlen(char *str);
int					get_next_line(int fd, char **line);
char				**ft_split(char *str, char *wp);
int					free_split(char **arr);
int					split_count(char **arg);

/*
**	Fonction de parsing
*/

int		parsing(int fd, t_data *data);
int		get_int(char *str, int *n);
int		get_double(char *str, double *d);
int		pars_color(char *str, t_color *color);
int		pars_point(char *str, t_point *point);
int		pars_vector(char *str, t_vector *v);

/*
**	Fonction de distance
*/

int		dist_init(t_data *d, t_ldist *p);

/*
**
*/

int		cam_gen(t_data *data, t_cam *c);

/*
**	Fonction de buffer
*/

t_buffer	*buf_create(int size);
int			buf_dest(t_buffer *b);
int			buf_addchar(t_buffer *b, char c);
char		*buf_to_string(t_buffer *b);

/*
**			Fonction de vector
*/

void		vect_set(t_vector *v, int x, int y, int z);
double		vect_norm(t_vector *v);
void		vect_tonorm(t_vector *v);
t_vector	vect_vect(t_vector *u, t_vector *v);
double		vect_dot(t_vector *u, t_vector *v);

t_point		vect_translate(t_point *p, t_vector *v);
t_vector	vect_vector(t_point *p1, t_point *p2);
void		vect_add(t_vector *v1, t_vector *v2);
int			vect_is_col(t_vector *v1, t_vector *v2);
t_vector	vect_def(double x, double y, double z);
t_vector	vect_mult(t_vector *v, double x);
t_point		vect_set_p(double x, double y, double z);

/*
**			parse_object
*/

int			pars_a(char **arr, t_data *data);
int			pars_res(char **arr, t_data *data);
int			pars_cam(char **arr, t_data *data);
int			pars_light(char **arr, t_data *data);
int			pars_sp(char **arr, t_data *data);
int			pars_tr(char **arr, t_data *data);
int			pars_pl(char **ar, t_data *data);

/*
**			Fonction de tranformation/Verification
*/

int			transform(t_list *l);
int			transform_cam(t_list *l, t_data *data);
int			transform_light(t_list *l);

/*
**			Fonction de resolution d'equation
*/

int			m_pow(int a, int b);
double		equa_premier(double a, double b);
double		equa_second(double a, double b, double c);

/*
**			Fonction d intersection
*/

int			inter_inter(t_ray *r, t_ldist *l, int i);

/*
**
*/

t_color		color_switch(t_data *data, t_ray *r);

/*
**			Fonction light
*/


int		light_add(t_lighting *li, t_color *color, double a);
int		light_init(t_lighting *li, t_plot *plot, t_color *color, double b);

/*
**			fonction de test
*/

void test_data_print(t_data *data);
void test_print_point(t_point *p);
void test_print_dist(t_ldist *p);
void test_print_color(t_color *c);
void test_print_split(char **arr);

int		ft_init_view(t_data *data);
int		ft_init_win(t_data *data, char *name);
int		ft_test_circle(t_data *data);
#endif
