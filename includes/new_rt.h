/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_rt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 15:10:50 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/22 18:59:45 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NEW_RT_H
# define NEW_RT_H

# include "scene.h"
//# include "ui.h"

typedef struct s_data
{
	t_scene		scene;
//	t_mlx		mlx;
//	t_ui		ui;
}	t_data;
/***
 * check the arguments given to the program
 * file must have a .rt extension
 * return SUCCESS if ok, ERR_INVALID_ARGS otherwise
 * @param argc number of arguments
 * @param argv array of arguments
 * @return int
 */
int		check_args(int argc, char **argv);

int		load_scene(const char *path, t_scene *out_scene);
int		run_app(t_data *data);

void	free_scene(t_scene *scene);
#endif


////////a suprimer :

typedef struct s_data
{
	t_scene		scene;
	t_engine	engine;
	t_display	display;
}	t_data;

//cette partie est ok pour l'instant
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



typedef struct s_display
{
	t_ui		ui;
	
}	t_display;

typedef struct s_framebuffer
{
	int		width;
	int		height;
	int		*pixels; /* RGB8 ou float selon ton choix */
}	t_framebuffer;

typedef struct s_cam_view
{
	const t_camera	*src;
	t_vec3f	origin;
	t_vec3f	forward;
	t_vec3f	right;
	t_vec3f	up;
	t_vec3f	p0;
	t_vec3f	dx;
	t_vec3f	dy;
	t_framebuffer	buffer;
	int		dirty;
}	t_cam_view;


typedef struct s_engine
{
	int			width;
	int			height;
	t_cam_view	*cam_views;

	//Les flag dont je parlais :
	int			active_cam;
	int			render_mode;
	int			samples_per_pixel;

	/* pas plus tard maintenant threads, tiles, stats, etc */
	
}	t_engine;

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

