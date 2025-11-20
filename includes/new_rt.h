/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_rt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 15:10:50 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/20 14:36:54 by kearmand         ###   ########.fr       */
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