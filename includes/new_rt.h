/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_rt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 15:10:50 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/27 20:22:41 by kearmand         ###   ########.fr       */
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

/***
 * load a scene from a .rt file
 * return SUCCESS if ok, error code otherwise
 * @param path path to the .rt file
 * @param out_scene pointer to the scene to fill
 * @return int
 */
int		load_scene(const char *path, t_scene *out_scene);

/***
 * run the main application loop
 * return SUCCESS if ok, error code otherwise
 * @param data pointer to the main data structure
 * @return int
 */
int		run_app(t_data *data);

void	free_data(t_data *data);
#endif
