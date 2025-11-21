/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 10:28:10 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/21 16:24:13 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

static int	vector_grow(t_vector *vector, size_t min_capacity)
{
	size_t			new_capacity;
	unsigned char	*new_data;
	size_t			bytes;
	size_t			i;

	new_capacity = vector->capacity;
	if (new_capacity == 0)
		new_capacity = 1;
	while (new_capacity < min_capacity)
		new_capacity *= 2;
	new_data = (unsigned char *)malloc(new_capacity * vector->element_size);
	if (!new_data)
		return (-1);
	bytes = vector->count * vector->element_size;
	i = 0;
	while (i < bytes)
	{
		new_data[i] = vector->data[i];
		i++;
	}
	free(vector->data);
	vector->data = new_data;
	vector->capacity = new_capacity;
	return (0);
}

int	vector_reserve(t_vector *vector, size_t min_capacity)
{
	if (min_capacity <= vector->capacity)
		return (0);
	return (vector_grow(vector, min_capacity));
}

int	vector_push_back(t_vector *vector, const void *element)
{
	size_t	offset;
	size_t	i;

	if (vector->count == vector->capacity)
		if (vector_grow(vector, vector->count + 1) != 0)
			return (-1);
	offset = vector->count * vector->element_size;
	i = 0;
	while (i < vector->element_size)
	{
		vector->data[offset + i] = ((const unsigned char *)element)[i];
		i++;
	}
	vector->count++;
	return (0);
}

void	*vector_emplace_back(t_vector *vector)
{
	size_t	offset;

	if (vector->count == vector->capacity)
		if (vector_grow(vector, vector->count + 1) != 0)
			return (NULL);
	offset = vector->count * vector->element_size;
	vector->count++;
	return (vector->data + offset);
}
