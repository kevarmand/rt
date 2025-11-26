/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bitmap.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 20:37:21 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/25 20:43:16 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BITMAP_H
# define BITMAP_H

# include <stdint.h>

typedef struct s_bitmap
{
	uint64_t	*words;
	int			word_count;
	int			bit_count;
}	t_bitmap;

int		bitmap_init(t_bitmap *bmp, int bit_count);
void	bitmap_destroy(t_bitmap *bmp);
void	bitmap_set(t_bitmap *bmp, int index);
void	bitmap_clear(t_bitmap *bmp, int index);
int		bitmap_get(const t_bitmap *bmp, int index);
void	bitmap_zero(t_bitmap *bmp);

#endif
