/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log_info_lines.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 03:20:59 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/17 03:39:33 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "log_info_internal.h"

void	print_title_line(const char *path)
{
	int	visible_len;

	visible_len = ft_strlen("PARSED SCENE SUMMARY") + 3 + ft_strlen(path);
	print_centered_prefix(visible_len);
	ft_putstr_fd(C_TITLE ANSI_BOLD, 1);
	ft_putstr_fd("PARSED SCENE SUMMARY", 1);
	ft_putstr_fd(ANSI_UNBOLD LOG_RESET, 1);
	ft_putstr_fd(" : ", 1);
	ft_putstr_fd(C_TITLE ANSI_BOLD, 1);
	ft_putstr_fd(path, 1);
	ft_putstr_fd(ANSI_UNBOLD LOG_RESET, 1);
	print_centered_suffix(visible_len);
}

void	print_resolution_line(int width, int height)
{
	int	visible_len;

	visible_len = ft_strlen("Resolution") + 2 + int_len(width)
		+ 3 + int_len(height);
	print_line_left_begin();
	ft_printf(C_RES "Resolution" LOG_RESET ": " ANSI_BOLD "%d" ANSI_UNBOLD
		" x " ANSI_BOLD "%d" ANSI_UNBOLD, width, height);
	print_line_left_end(visible_len);
}

void	print_cam_light_line(int cameras, int lights)
{
	int	lw;
	int	rw;
	int	llen;
	int	rlen;

	lw = col_left_width();
	rw = col_right_width();
	llen = ft_strlen("Cameras") + 2 + int_len(cameras);
	rlen = ft_strlen("Lights") + 2 + int_len(lights);
	print_line_cols_begin();
	print_col_center_prefix(lw, llen);
	ft_printf(C_CAM "Cameras" LOG_RESET ": " ANSI_BOLD "%d" ANSI_UNBOLD,
		cameras);
	print_col_center_suffix(lw, llen);
	print_col_center_prefix(rw, rlen);
	ft_printf(C_LGT "Lights" LOG_RESET ": " ANSI_BOLD "%d" ANSI_UNBOLD, lights);
	print_col_center_suffix(rw, rlen);
	print_line_cols_end();
}

void	print_mtsb_line(const t_scene *scene)
{
	int	lw;
	int	rw;
	int	llen;
	int	rlen;

	lw = col_left_width();
	rw = col_right_width();
	llen = ft_strlen("Materials") + 2 + int_len(scene->material_count)
		+ 4 + ft_strlen("Surfaces") + 2 + int_len(scene->surface_count);
	rlen = ft_strlen("Textures") + 2 + int_len(scene->texture_count)
		+ 4 + ft_strlen("Bumpmaps") + 2 + int_len(scene->bumpmap_count);
	print_line_cols_begin();
	print_col_center_prefix(lw, llen);
	ft_printf(C_MAT "Materials" LOG_RESET ": " ANSI_BOLD "%d" ANSI_UNBOLD
		"    " C_SURF "Surfaces" LOG_RESET ": " ANSI_BOLD "%d" ANSI_UNBOLD,
		scene->material_count, scene->surface_count);
	print_col_center_suffix(lw, llen);
	print_col_center_prefix(rw, rlen);
	ft_printf(C_TEX "Textures" LOG_RESET ": " ANSI_BOLD "%d" ANSI_UNBOLD
		"    " C_BUMP "Bumpmaps" LOG_RESET ": " ANSI_BOLD "%d" ANSI_UNBOLD,
		scene->texture_count, scene->bumpmap_count);
	print_col_center_suffix(rw, rlen);
	print_line_cols_end();
}

void	print_objects_line(int prim, int planes)
{
	int	total;
	int	visible_len;

	total = prim + planes;
	visible_len = ft_strlen("Objects (") + int_len(total)
		+ ft_strlen("): Planes (") + int_len(planes)
		+ ft_strlen(") + Primitives (") + int_len(prim)
		+ ft_strlen(")");
	print_centered_prefix(visible_len);
	ft_printf(C_OBJ ANSI_BOLD "Objects" ANSI_UNBOLD LOG_RESET
		" (" ANSI_BOLD "%d" ANSI_UNBOLD "): "
		"Planes (" ANSI_BOLD "%d" ANSI_UNBOLD ") + "
		"Primitives (" ANSI_BOLD "%d" ANSI_UNBOLD ")",
		total, planes, prim);
	print_centered_suffix(visible_len);
}
