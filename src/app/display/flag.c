/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flag.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 19:59:02 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/23 20:05:28 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	flag_has(int flags, int mask)
{
	return (!!(flags & mask));
}

void	flag_set(int *flags, int mask)
{
	*flags |= mask;
}

void	flag_clear(int *flags, int mask)
{
	*flags &= ~mask;
}
