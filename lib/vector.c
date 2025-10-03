#include <stdlib.h>
#include "vector.h"

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

#include <stdio.h>
int main()
{
	t_vector vector;

	if (vector_init(&vector, 4, sizeof(char)) != 0)
		return (1);
	for (int i = 0; i < 1000; i++)
	{
		char c = (char)(i % 256);
		vector_push_back(&vector, &c);
	}
	printf("Size: %zu, Capacity: %zu\n", vector_size(&vector), vector_capacity(&vector));
	for (size_t i = 0; i < vector_size(&vector); i++)
	{
		char *c = (char *)vector_at(&vector, i);
		printf("%d ", (unsigned char)(*c));
	}
	vector_destroy(&vector);
	return 0;
}