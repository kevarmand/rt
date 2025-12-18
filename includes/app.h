/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 20:49:12 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/18 13:42:40 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef APP_H
# define APP_H


typedef struct s_data	t_data;


/* ************************************************************************** */
/*         					     HANDLERS                                     */
/* ************************************************************************** */


int	run_app(t_data *data);
int	frame_tick(t_data *data);

int	sync_engine_tick(t_data *data);

int	key_hook(int keycode, void *param);
int	mouse_press(int button, int x, int y, void *param);
int	mouse_move(int x, int y, void *param);
int	mouse_release(int button, int x, int y, void *param);


#endif