/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_display.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 18:39:42 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/28 13:44:34 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "manager.h"
#include "libft.h"
#include <stdio.h>
int	manager_update_display(t_render *render)
{
	t_mgr				*manager;
	t_display_mailbox	*mailbox;
	int					pixel_count;

	manager = &render->manager;
	mailbox = &render->mailbox;
	if (atomic_load(&mailbox->snapshot_ready) != 0)
		return (0);
	pixel_count = render->width * render->height;
	ft_memcpy(mailbox->rgb_pixels, manager->rgb_buffer,
		(size_t)pixel_count * sizeof(int));
	mailbox->tile_count = manager->tileset.tiles_total;
	mailbox->tiles_done = manager->tileset.tiles_done;
	atomic_store(&mailbox->snapshot_ready, 1);
	return (1);
}
