/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 10:30:08 by kearmand          #+#    #+#             */
/*   Updated: 2025/10/06 10:30:17 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"

void	*vector_data(t_vector *vector)
{
	return (vector->data);
}

size_t	vector_size(t_vector *vector)
{
	return (vector->count);
}

size_t	vector_capacity(t_vector *vector)
{
	return (vector->capacity);
}

void	*vector_at(t_vector *vector, size_t index)
{
	return (vector->data + index * vector->element_size);
}
