/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bitmap.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 20:38:59 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/28 10:39:43 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bitmap.h"

#include <stdlib.h>
#include "bitmap.h"
#include "errors.h"

int	bitmap_init(t_bitmap *bmp, int bit_count)
{
	int	word_count;
	int	index;

	bmp->bit_count = bit_count;
	word_count = bit_count / 64;
	if (bit_count % 64 != 0)
		word_count++;
	bmp->word_count = word_count;
	bmp->words = malloc(sizeof(uint64_t) * word_count);
	if (bmp->words == NULL)
		return (ERR_MALLOC);
	index = 0;
	while (index < word_count)
	{
		bmp->words[index] = 0;
		index++;
	}
	return (0);
}

void	bitmap_destroy(t_bitmap *bmp)
{
	if (bmp->words != NULL)
		free(bmp->words);
	bmp->words = NULL;
	bmp->word_count = 0;
	bmp->bit_count = 0;
}

void	bitmap_zero(t_bitmap *bmp)
{
	int	index;

	index = 0;
	while (index < bmp->word_count)
	{
		bmp->words[index] = 0;
		index++;
	}
}

void	bitmap_set(t_bitmap *bmp, int index)
{
	int	word_index;
	int	bit_index;

	word_index = index / 64;
	bit_index = index % 64;
	bmp->words[word_index] |= ((uint64_t)1 << bit_index);
}
void	bitmap_clear(t_bitmap *bmp, int index)
{
	int	word_index;
	int	bit_index;

	word_index = index / 64;
	bit_index = index % 64;
	bmp->words[word_index] &= ~((uint64_t)1 << bit_index);
}

int	bitmap_get(const t_bitmap *bmp, int index)
{
	int	word_index;
	int	bit_index;

	word_index = index / 64;
	bit_index = index % 64;
	return ((bmp->words[word_index] >> bit_index) & 1);
}
