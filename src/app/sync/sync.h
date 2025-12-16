/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sync.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 18:46:42 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/16 20:30:51 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYNC_H
# define SYNC_H

# include "new_rt.h"
# include "display.h"
# include "engine.h"
# include "render.h"
# include "libft.h"
# include <stdatomic.h>
# include <stdio.h>

void		display_request_policy(const t_scene *scene, t_display *display);
void		display_engine_send(t_scene *scene, t_display *display,
				t_display_mailbox *mailbox);
void		display_engine_receive(t_display *display,
				t_display_mailbox *mailbox);

t_quality	get_active_target_quality(t_quality current_quality,
				t_user_render_mode user_mode);
t_quality	get_background_target_quality(t_quality current_quality,	
				t_user_render_mode user_mode);

#endif