/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   synch_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 18:15:39 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/16 20:29:38 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sync.h"

static t_quality	lock_target_quality(t_user_render_mode user_mode)
{
	if (user_mode == USER_RENDER_LOCK_FAST)
		return (QUALITY_FAST);
	if (user_mode == USER_RENDER_LOCK_NORMAL)
		return (QUALITY_NORMAL);
	if (user_mode == USER_RENDER_LOCK_SUPER)
		return (QUALITY_SUPER);
	return (QUALITY_NONE);
}

t_quality	get_active_target_quality(t_quality current_quality,
			t_user_render_mode user_mode)
{
	t_quality	target_quality;

	if (user_mode == USER_RENDER_AUTO)
	{
		if (current_quality < QUALITY_FAST)
			return (QUALITY_FAST);
		if (current_quality < QUALITY_NORMAL)
			return (QUALITY_NORMAL);
		return (QUALITY_NONE);
	}
	target_quality = lock_target_quality(user_mode);
	if (target_quality != QUALITY_NONE && current_quality < target_quality)
		return (target_quality);
	return (QUALITY_NONE);
}

t_quality	get_background_target_quality(t_quality current_quality,
					t_user_render_mode user_mode)
{
	if (user_mode == USER_RENDER_AUTO)
	{
		if (current_quality < QUALITY_NORMAL)
			return (QUALITY_NORMAL);
		return (QUALITY_NONE);
	}
	if (user_mode == USER_RENDER_LOCK_FAST)
	{
		if (current_quality < QUALITY_FAST)
			return (QUALITY_FAST);
		return (QUALITY_NONE);
	}
	if (user_mode == USER_RENDER_LOCK_NORMAL)
	{
		if (current_quality < QUALITY_NORMAL)
			return (QUALITY_NORMAL);
		return (QUALITY_NONE);
	}
	if (user_mode == USER_RENDER_LOCK_SUPER)
	{
		if (current_quality < QUALITY_SUPER)
			return (QUALITY_SUPER);
		return (QUALITY_NONE);
	}
	return (QUALITY_NONE);
}
