/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 10:08:49 by kearmand          #+#    #+#             */
/*   Updated: 2025/10/06 10:33:04 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_H
# define VECTOR_H
# include <stddef.h>

typedef struct s_vector
{
	unsigned char	*data;
	size_t			element_size;
	size_t			count;
	size_t			capacity;
}	t_vector;

int		vector_init(t_vector *vector, size_t initial_capacity,
			size_t element_size);
void	vector_destroy(t_vector *vector);
void	*vector_data(t_vector *vector);
size_t	vector_size(t_vector *vector);
size_t	vector_capacity(t_vector *vector);
void	*vector_at(t_vector *vector, size_t index);
int		vector_reserve(t_vector *vector, size_t min_capacity);
int		vector_push_back(t_vector *vector, const void *element);
void	*vector_emplace_back(t_vector *vector);
int		vector_remove_stable(t_vector *vector, size_t index);
int		vector_remove_swap(t_vector *vector, size_t index);
void	vector_clear(t_vector *vector);

#endif