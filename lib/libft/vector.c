/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 10:10:06 by kearmand          #+#    #+#             */
/*   Updated: 2025/10/06 10:30:04 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "vector.h"

int	vector_init(t_vector *vector, size_t initial_capacity, size_t element_size)
{
	vector->data = NULL;
	vector->element_size = element_size;
	vector->count = 0;
	vector->capacity = 0;
	if (initial_capacity == 0)
		return (0);
	vector->data = (unsigned char *)malloc(initial_capacity * element_size);
	if (!vector->data)
		return (-1);
	vector->capacity = initial_capacity;
	return (0);
}

void	vector_destroy(t_vector *vector)
{
	if (vector->data)
		free(vector->data);
	vector->data = NULL;
	vector->element_size = 0;
	vector->count = 0;
	vector->capacity = 0;
}

int	vector_remove_stable(t_vector *vector, size_t index)
{
	size_t	from_off;
	size_t	to_off;
	size_t	tail_bytes;
	size_t	i;

	if (index + 1 < vector->count)
	{
		from_off = (index + 1) * vector->element_size;
		to_off = index * vector->element_size;
		tail_bytes = (vector->count - index - 1) * vector->element_size;
		i = 0;
		while (i < tail_bytes)
		{
			vector->data[to_off + i] = vector->data[from_off + i];
			i++;
		}
	}
	if (vector->count > 0)
		vector->count--;
	return (0);
}

int	vector_remove_swap(t_vector *vector, size_t index)
{
	size_t	dst_off;
	size_t	src_off;
	size_t	i;

	if (index + 1 < vector->count)
	{
		dst_off = index * vector->element_size;
		src_off = (vector->count - 1) * vector->element_size;
		i = 0;
		while (i < vector->element_size)
		{
			vector->data[dst_off + i] = vector->data[src_off + i];
			i++;
		}
	}
	if (vector->count > 0)
		vector->count--;
	return (0);
}

void	vector_clear(t_vector *vector)
{
	vector->count = 0;
}
