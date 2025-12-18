/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 22:02:51 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/18 22:02:58 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UI_H
# define UI_H

# include "ui_settings.h"

void	init_ui(t_data *data);
void	print_txt_ui(t_data *data);
int		add_button(t_ui *ui, t_button btn);
void	draw_all_buttons(t_ui *ui);
void	print_all_button_txt(t_data *data);
void	create_interface(t_data *data);
int		draw_rectangle(t_image *img, t_rect *rect);
void	create_buttons(t_ui *ui, t_data *data);
void	draw_escape_cross(t_image *img);
void	put_pixel_to_image(t_image *img, int x, int y, int color);
void	close_ui(t_data *data);
int		add_button(t_ui *ui, t_button btn);

void	reset_ui(t_data *data);

// Button actions
void	create_tab(t_ui *ui, t_data *data);
void	create_descr(t_ui *ui, t_data *data);

#endif