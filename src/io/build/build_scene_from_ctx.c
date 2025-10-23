

/***
 * Objectif de la partie convertir nos cx en un scene grossomodo on a jsute a 
 * "suprimer les hashmap" et change le ownership des vecteur  ( on fait un realloc propre pour la continuite ?)
 */

#include "convert.h"
#include "scene.h"
#include "error_codes.h"
#include "libft.h"

int	build_scene_from_ctx(t_scene *scene, t_ctx *ctx)
{
	if (!scene || !ctx)
		return (ERROR);
	
	
	return (SUCCESS);
}

//je ferai bien un petit helper qui :
// - qui prend l adresse d un vecteur,
// - qui prend l adresse d une hashmap (optionnel)
// - qui prend l adresse d un compteur
//et qui fait le boulot de transfert proprement ( malloc + copie + free hashmap si besoin

//on va lappeler : build_array_from_vector

int	build_array_from_vector(void **out_array, t_vector *vec, t_hashmap **map,
									int *out_count, size_t elem_size)
{
	size_t	vec_size;
	void	*array;

	if (!out_array || !vec || !out_count || elem_size == 0)
		return (ERROR);
	vec_size = vec->count;
	array = malloc(elem_size * vec_size);
	if (!array)
		return (ERR_MALLOC);
	ft_memcpy(array, vec->data, elem_size * vec_size);
	vector_destroy(vec);
	if (*map)
	{
		hashmap_destroy(*map, NULL);
		*map = NULL;
	}
	*out_array = array;
	*out_count = (int)vec_size;
	return (SUCCESS);
}
