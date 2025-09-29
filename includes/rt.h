/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/10 13:22:08 by karmand           #+#    #+#             */
/*   Updated: 2025/09/11 13:18:13 by kearmand         ###   ########.fr       */
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
# include "type.h"
# include "ui.h"

# include <X11/X.h>
# include "error_codes.h"

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

# define FLAG_DRAW_UI      0x01  // Redessiner l'interface
# define FLAG_DRAW_VIEW    0x02  // Recalculer l'image raytracée
# define FLAG_DRAW_TEXT    0x04  // Redessiner le texte
# define FLAG_DRAW_RESET_UI 0x08  // Réinitialiser l'interface
# define FLAG_DRAW_ALL     0x0F  // Redessiner tout

# define ERR_EMPTY_OBJ		-16

# define ERR_NORM			-300


/*
**	Fonction de liste chainee
*/

int					list_addback(t_objlist **l, void *p);
int					list_addfront(t_objlist **l, void *p, t_type t);
int					list_count(t_objlist *l);
int					list_free(t_objlist *l);

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
char				*ft_strjoin2(const char *s1, const char *s2);
char				*ft_itoa(int n);


/*
**		Fonction de parsing
*/

int					parsing(int fd, t_data *data);
int					get_int(char *str, int *n);
int					get_double(char *str, double *d);
int					pars_color(char *str, t_color *color);
int					pars_point(char *str, t_point *point);
int					pars_vector(char *str, t_vec3 *v);

void				name_lst(t_objlist *lst);
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

t_vec3			translate_type(int type);
int					translate_plane(t_pl *pl, int type);
int					translate_sphere(t_sp *sp, int type);
int					translate_square(t_sq *sq, int type);
int					translate_triangle(t_tr *tr, int type);
int					translate_cylinder(t_cy *cy, int type);
int					translate_cam(t_cam *cam, int type);
int					translate_switch(int type, t_data *data);
int					translate_cam(t_cam *cam, int type);
int					translate_light(t_light *light, int type);

int					rot_vect(t_vec3 *v, int type);
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

void				vect_set(t_vec3 *v, int x, int y, int z);
double				vect_norm(t_vec3 *v);
void				vect_tonorm(t_vec3 *v);
t_vec3			vect_vect(t_vec3 *u, t_vec3 *v);
double				vect_dot(t_vec3 *u, t_vec3 *v);

t_point				vect_translate(t_point *p, t_vec3 *v);
t_vec3			vect_vector(t_point *p1, t_point *p2);
void				vect_add(t_vec3 *v1, t_vec3 *v2);
void				vect_sub(t_vec3 *v1, t_vec3 *v2);
int					vect_is_col(t_vec3 *v1, t_vec3 *v2);
t_vec3			vect_def(double x, double y, double z);
t_vec3			vect_mult(t_vec3 *v, double x);
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
void				mat_prod_vect(t_vec3 *v, double m[3][3], t_vec3 *u);
void				mat_gen_pass(double m[3][3], t_vec3 *v1, t_vec3 *v2);
void				mat_transpose(double r[3][3], double m[3][3]);

/*
**			Fonction d intersection
*/
double				inter_triangle(t_point *p, t_vec3 *v, t_tr *tr);
double				inter_cylinder(t_point *p, t_vec3 *v, t_cy *cy);
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

/***
 *  Fontion de gestion des evenements
 */
int					key_hook(int keycode, t_data *data);
int					mouse_press(int button, int x, int y, t_data *data);
int					mouse_release(int button, int x, int y, t_data *data);
int					mouse_move(int x, int y, t_data *data);



/***
 * Fonction d init1
 */
int	 frame_tick(t_data *data);
int	render_tick(t_data *data);
t_color		cam_getcolor(t_data *data, t_cam *c, t_vec3 *v);
int	 frame_tick(t_data *data);
int	render_tile(t_data *data, t_tile *tile, t_cam *cam);
void	render_begin(t_data *data, t_cam *cam);

#endif