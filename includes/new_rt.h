/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_rt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 15:10:50 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/23 19:22:21 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NEW_RT_H
# define NEW_RT_H

# include "scene.h"
# include "engine.h"
# include "display.h"
//# include "ui.h"


typedef struct s_data
{
	t_scene		scene;
	t_engine	engine;
	t_display	display;
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


