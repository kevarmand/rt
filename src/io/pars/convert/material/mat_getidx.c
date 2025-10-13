#include "io.h"
#include "rt.h"
#include "error_codes.h"
#include "scene.h"
#include "libft.h"
#include <stdint.h>

#define KEY_SIZE 128

void	generate_option_key(const t_element_options *opts, char *key);

static void	init_material_from_options(t_material *mat, const t_element_options *opts)
{
	mat->ambient = opts->ambient_occlusion;
	mat->diffuse = opts->diffuse_weight;
	mat->specular = opts->specular_weight;
	mat->shininess = opts->shininess;
	mat->reflection = opts->reflection;
	mat->refraction = opts->refraction;
	mat->ior = opts->ior;
	mat->texture_albedo_id = SCENE_ID_NONE;
	mat->texture_normal_id = SCENE_ID_NONE;
}

/**
 * @brief Retrieves the index of a material matching the given options,
 *        or creates a new one if it does not exist yet.
 *
 * This function generates a deterministic key from the material options,
 * then checks whether a material with identical parameters is already
 * stored in the hashmap. If found, its index is returned.
 * Otherwise, a new material is built from the options, appended to the
 * material vector, and inserted into the hashmap.
 *
 * Because NULL is used internally to mark empty hashmap slots,
 * stored indices are shifted by +1 when inserted, and decremented
 * by 1 when retrieved. This prevents index 0 from being misinterpreted
 * as a NULL entry.
 *
 * @param materials   Pointer to the dynamic array (vector) of materials.
 * @param mat_map     Pointer to the hashmap used for fast material lookup.
 * @param opts        Pointer to the material options structure.
 *
 * @return The index of the existing or newly created material on success,
 *         or -1 if any allocation or insertion fails.
 */
int	mat_getidx(t_vector *materials,
				t_hashmap *mat_map,
				const t_element_options *opts)
{
	char		key[KEY_SIZE];
	int			index_hash;
	t_material	new_mat;
	char 		*stored_key;
	int			index_new_mat;

	generate_option_key(opts, key);
	index_hash = (int)(intptr_t)(hashmap_get(mat_map, key)) - 1;
	if (index_hash >= 0)
		return (index_hash);
	init_material_from_options(&new_mat, opts);
	if (vector_push_back(materials, &new_mat) != 0)
		return (-1);
	index_new_mat = (int)(materials->count - 1);
	stored_key = ft_strdup(key);
	if (stored_key == NULL)
		return (-1);
	if (hashmap_insert(mat_map, stored_key, (void *)(intptr_t)(1 + index_new_mat)) != 0)
	{
		free(stored_key);
		return (-1);
	}
	return (index_new_mat);
}
