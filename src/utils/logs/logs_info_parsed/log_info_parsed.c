/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log_info_parsed.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 03:21:26 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/17 03:40:52 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "log_info_internal.h"

void	log_info_parsed(const t_scene *scene, const char *path)
{
	ft_putchar_fd('\n', 1);
	print_edge(UTF8_ULCORNER, UTF8_HLINE, UTF8_URCORNER);
	print_inner_hline();
	print_title_line(path);
	print_inner_hline();
	print_resolution_line(scene->resolution_width, scene->resolution_height);
	print_empty_line();
	print_cam_light_line(scene->camera_count, scene->light_count);
	print_mtsb_line(scene);
	print_empty_line();
	print_objects_line(scene->primitive_count, scene->plane_count);
	print_edge(UTF8_LLCORNER, UTF8_HLINE, UTF8_LRCORNER);
	ft_putchar_fd('\n', 1);
}
