/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log_info_internal.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 03:06:52 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/17 13:00:36 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOG_INFO_INTERNAL_H
# define LOG_INFO_INTERNAL_H

# include "logs.h"
# include "scene.h"
# include "libft.h"

/* Layout */
# define LOG_INFO_W 96

/* UTF8 box */
# define UTF8_VLINE    "\xE2\x94\x82" //│|
# define UTF8_HLINE    "\xE2\x94\x80"//─
# define UTF8_ULCORNER "\xE2\x94\x8C"//┌
# define UTF8_URCORNER "\xE2\x94\x90"//┐
# define UTF8_LLCORNER "\xE2\x94\x94"//└
# define UTF8_LRCORNER "\xE2\x94\x98"//┘

/* ANSI */
# define ANSI_BOLD   "\033[1m"
# define ANSI_UNBOLD "\033[22m"

/* Colors */
# define C_FRAME  LOG_RESET
# define C_TITLE  LOG_CYAN
# define C_RES    LOG_GRAY
# define C_CAM    LOG_GREEN
# define C_LGT    LOG_ORANGE
# define C_MAT    LOG_MAGENTA
# define C_SURF   LOG_BLUE
# define C_TEX    LOG_YELLOW
# define C_BUMP   LOG_YELLOW
# define C_OBJ    LOG_RED

int		inner_width(void);
int		col_left_width(void);
int		col_right_width(void);
void	print_line_cols_begin(void);
void	print_line_cols_end(void);
void	print_col_center_prefix(int col_w, int content_len);
void	print_col_center_suffix(int col_w, int content_len);
void	print_line_left_begin(void);
void	print_line_left_end(int content_len);
void	print_centered_prefix(int visible_len);
void	print_centered_suffix(int visible_len);
void	print_inner_hline(void);
void	print_edge(const char *l, const char *h, const char *r);
void	print_empty_line(void);
int		int_len(int value);
void	put_nchar(char ch, int count);
void	put_nstr(const char *str, int count);

//print functions for each line
void	print_title_line(const char *path);
void	print_resolution_line(int width, int height);
void	print_cam_light_line(int cameras, int lights);
void	print_mtsb_line(const t_scene *scene);
void	print_objects_line(int prim, int planes);

#endif