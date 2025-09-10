/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_vec_int.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 15:49:32 by kearmand          #+#    #+#             */
/*   Updated: 2025/05/31 15:51:25 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	vector_int_init(t_vector_int *vec, size_t initial_capacity)
{
	vec->data = malloc(sizeof(int) * initial_capacity);
	if (!vec->data)
		return (-1);
	vec->count = 0;
	vec->capacity = initial_capacity;
	return (0);
}

void	vector_int_destroy(t_vector_int *vec)
{
	if (vec->data)
		free(vec->data);
	vec->data = NULL;
	vec->count = 0;
	vec->capacity = 0;
}

int	vector_int_push_back(t_vector_int *vec, int value)
{
	int		*new_data;
	size_t	new_capacity;
	size_t	i;

	if (vec->count >= vec->capacity)
	{
		new_capacity = vec->capacity * 2 + 1;
		new_data = malloc(new_capacity * sizeof(int));
		if (!new_data)
			return (-1);
		i = 0;
		while (i < vec->count)
		{
			new_data[i] = vec->data[i];
			i++;
		}
		free(vec->data);
		vec->data = new_data;
		vec->capacity = new_capacity;
	}
	vec->data[vec->count++] = value;
	return (0);
}
