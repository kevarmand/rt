#ifndef TYPE_H
# define TYPE_H

# define MAX_UI_BUTTONS 40


/***
 * struct for the norm 42 t_rect
 */
typedef struct s_rect
{
	int color;
	int width;
	int height;
	int x;
	int y;
}	t_rect;

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
	char			*name;
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
	int				endian;
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

typedef struct s_button
{
	int		x;           // relatif à l'UI
	int		y;
	int		width;
	int		height;
	int		label;
	char	*txt;
	int		color_bg;
	int		color_txt;
	void	(*on_click)(void *data, int flag); // action à effectuer
}	t_button;

typedef struct s_ui {
	int		visible;
	t_image	img;
	int		x;
	int		y;
	int		dragging;
	int		drag_offset_x;
	int		drag_offset_y;
	t_button	buttons[MAX_UI_BUTTONS]; // tableau de boutons
	int		button_count;
}	t_ui;

typedef struct		s_data
{
	t_para			para;
	t_list			*lobj;
	t_list			*llight;
	t_list			*lcam;
	t_select		select;
	t_ldist			ldist;
	t_ui			ui;
	void			*mlx_ptr;
	void			*win_ptr;
	t_image			*view;
	int				test[3];
	int				flag_draw;
}					t_data;

#endif