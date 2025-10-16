#include "io.h"
#include "rt.h"
#include "error_codes.h"
#include "scene.h"
#include "libft.h"
#include <stdint.h>

//On convertit les texture en id : on les ouvrira plus tard la cest juste de la reconnaissance semantique

int conv_texture(t_scene *scene, const char *path, int *out_id)
{
	//1. chercher dans la hashmap si cette texture existe deja
	int index_hash;
	index_hash = (int)(intptr_t)(hashmap_get(&scene->texture_map, path));
	if (index_hash > 0)
	{
		*out_id = index_hash - 1;
		return (SUCCESS);
	}
	//2. si non, creer une nouvelle texture, l ajouter au vector et a la hashmap
	t_texture new_texture;
	//2.a initialiser la texture ( les parametre juste la string )
	texture_init(&new_texture, path);
	if (vector_push_back(&scene->textures, &new_texture) != SUCCESS)
		return (ERR_ALLOC);
	int new_index = scene->textures.size - 1;
	if (hashmap_insert(&scene->texture_map, path, (void *)(intptr_t)(new_index + 1)) != SUCCESS)
	{
		//vector_pop(&scene->textures);  Je el met en commentaire : je compte el suprimer : si ona  une erreur on va juste vider la scene donc pas de fuite
		return (ERR_ALLOC);
	}
	*out_id = new_index;
	return (SUCCESS);
}
