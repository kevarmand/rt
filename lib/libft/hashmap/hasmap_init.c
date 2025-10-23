/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hasmap_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 15:10:37 by kearmand          #+#    #+#             */
/*   Updated: 2025/10/23 15:12:08 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/***
 * @brief Initialize a hashmap with a given initial capacity.
 * @param map The hashmap to initialize.
 * @param initial_capacity The initial capacity of the hashmap.
 * @return 0 on success, -1 on failure.
*/

int	hashmap_init(t_hashmap *map, size_t initial_capacity)
{
	map->nodes = (t_hashnode *)ft_calloc(initial_capacity, sizeof(t_hashnode));
	if (!map->nodes)
		return (-1);
	map->capacity = initial_capacity;
	map->count = 0;
	return (0);
}

void	hashmap_free_nodes(t_hashmap *map, void (*destroy)(void *))
{
	size_t	i;

	if (!map || !map->nodes)
		return ;
	i = 0;
	while (i < map->capacity)
	{
		if (map->nodes[i].key)
		{
			free(map->nodes[i].key);
			if (destroy && map->nodes[i].value)
				destroy(map->nodes[i].value);
		}
		i++;
	}
	free(map->nodes);
	map->nodes = NULL;
	map->capacity = 0;
	map->count = 0;
}
