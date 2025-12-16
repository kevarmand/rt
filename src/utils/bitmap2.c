/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bitmap2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 17:02:47 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/16 17:03:13 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bitmap.h"
#include <stdlib.h>
#include "bitmap.h"
#include "errors.h"

int	bitmap_get(const t_bitmap *bmp, int index)
{
	int	word_index;
	int	bit_index;

	word_index = index / 64;
	bit_index = index % 64;
	return ((bmp->words[word_index] >> bit_index) & 1);
}
