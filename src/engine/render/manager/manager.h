/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 15:15:03 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/17 23:33:14 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MANAGER_H
# define MANAGER_H

# include "render.h"
# include <unistd.h>

int		manager_collect_tiles(t_render *render);
int		manager_assign_jobs(t_render *render);
int		manager_convert_hdr_to_rgb(t_render *render);
int		manager_update_display(t_render *render);
int		manager_read_mailbox(t_render *render);
int		manager_run_tonemap(t_mgr *mgr, int width, int height);

#endif