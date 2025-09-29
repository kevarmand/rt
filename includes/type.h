#ifndef TYPE_H
# define TYPE_H

# define MAX_UI_BUTTONS 50
# define NB_CORE 24

# include <stdatomic.h>
# include <pthread.h>
# include <stdint.h>

# include <stdlib.h>
# include <string.h>
//forward declaration
struct s_data;
typedef struct s_data t_data;


struct s_render;
typedef struct s_render t_render;

struct s_tile;
typedef struct s_tile t_tile;

struct s_cam;
typedef struct s_cam t_cam;

/**
 * struct s_tile - Rendering unit assigned to a worker thread
 * @buffer:		Pixel buffer for this tile
 * @id:			Global index of the tile
 * @pos_x:		Tile position in X (pixels)
 * @pos_y:		Tile position in Y (pixels)
 * @camera:		Camera used for rendering this tile
 * @thread_id:	POSIX thread identifier
 * @is_done:	Atomic flag, set to 1 when rendering is finished
 */
struct  s_tile
{
	int			*buffer;
	int			id;
	int			pos_x;			// Unused for now
	int			pos_y;			// Unused for now
	t_cam		*camera;
	pthread_t	thread_id;
	atomic_int	is_done;
};

/**
 * struct s_render - Global rendering context
 * @width:				Output resolution in X (rounded to next multiple of TILE_SIZE)
 * @height:				Output resolution in Y (rounded to next multiple of TILE_SIZE)
 * @framebuffer:		HDR framebuffer (temporary int-based, to be replaced by float)
 * @camera:				Active camera for rendering
 * @samples_per_pixel:	Number of samples per pixel
 * @cancel_flag:		Atomic flag to abort rendering
 * @thread_next_id:		Next thread identifier to assign
 * @tiles:				Array of per-thread tile structures (size = NB_CORE)
 * @tiles_bitmap:		Bitset tracking finished tiles
 * @tiles_total:		Total number of tiles (constant, fixed at init)
 * @tiles_done:			Number of tiles already completed
 * @tiles_distributed:	Number of tiles already assigned to workers
 */
struct s_render{
	int			width;
	int			height;
	int			*framebuffer;
	t_cam		*camera;
	int			samples_per_pixel;		//unused for now
	atomic_int	cancel_flag;
	atomic_int	thread_next_id;
	t_tile		tiles[NB_CORE];
	uint64_t 	*tiles_bitmap;
	int			tiles_total;
	int			tiles_done;
	int			tiles_distributed;
};




// /***
//  * t_scene
//  */
// typedef struct s_scene
// {
// 	t_objlist	*objects;
// 	t_objlist	*cameras;
// 	t_objlist	*lights;
// 	t_para	globals;
// }	t_scene;


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

typedef struct		s_vec3
{
	double			x;
	double			y;
	double			z;
}					t_vec3;

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
	t_vec3		norm;
	t_color			color;
	double			a;
	double			b;
	double			c;
	double			d;
}					t_pl;

typedef struct		s_cy
{
	t_point			point;
	t_vec3		dir;
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
	t_vec3		norm;
	t_color			color;
}					t_tr;

typedef struct		s_sq
{
	t_point			point;
	t_vec3		norm;
	double			height;
	t_color			color;
	t_vec3		dir1;
	t_vec3		dir2;
	double			a;
	double			b;
	double			c;
	double			d;
}					t_sq;

typedef struct		s_cam
{
	t_point			point;
	t_vec3		dir;
	double			fov;
	t_vec3		hori;
	t_vec3		vert;
	t_point			first;
}					t_cam;

typedef struct		s_ray
{
	t_point			source;
	t_vec3		dir;
	int				inter;
	double			dist;
}					t_ray;

typedef struct		s_lighting
{
	t_point			inter;
	t_vec3		norm;
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

typedef struct		s_objlist
{
	void			*obj;
	t_type			type;
	char			*name;
	struct s_objlist	*next;
}					t_objlist;

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

typedef enum e_ui_tab {
	UI_TAB_OBJECT = 0,
	UI_TAB_LIGHT,
	UI_TAB_CAMERA
}	t_ui_tab;

typedef struct		s_select
{
	t_objlist			*obj;
	t_objlist			*light;
	t_objlist			*cam;
	int				active_tab;
}					t_select;

/*
**	pour les test
**	test[0] echap
**	test[1]	resolution
**	test[2]	ambiance
*/

typedef struct s_button
{
	int		x;           
	int		y;
	int		width;
	int		height;
	int		label;
	char	*txt;
	int		color_bg;
	int		color_txt;
	void	(*on_click)(t_data *data); // action à effectuer
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
	t_objlist			*lobj;
	t_objlist			*llight;
	t_objlist			*lcam;
	t_select		select;
	t_ldist			ldist;
	t_ui			ui;
	void			*mlx_ptr;
	void			*win_ptr;
	t_image			*view;
	int				test[3];
	int				flag_draw;
	t_render		render;
}					t_data;

#endif
