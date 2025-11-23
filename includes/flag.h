/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flag.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 20:01:14 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/23 20:03:19 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FLAG_H
# define FLAG_H

enum e_display_flag
{
	DF_UI = 1 << 0,
	DF_TEXT = 1 << 1,
	DF_ALL  = DF_UI | DF_TEXT
};

int		flag_has(int flags, int mask);
void	flag_set(int *flags, int mask);
void	flag_clear(int *flags, int mask);

#endif