#include "scene.h"
#include "convert.h"
#include "io.h"
#include "error_codes.h"
#include <stdlib.h>
#include "libft.h"

void	cleanup_str(char **s)
{
	free(s[0]);
	s[0] = NULL;
}

static void	init_default_texture(t_texture *tex)
{
	tex->path = NULL;
	tex->width = 0;
	tex->height = 0;
	tex->channels = 0;
	tex->pixels = NULL;
}

int	intern_texture(t_conv_ctx *cx, char **path, t_index *out_id)
{
	void		*found;
	t_texture	new_tex;
	int			insert_index;

	if (!*path)
	{
		*out_id = SCENE_ID_NONE;
		return (SUCCESS);
	}
	found = hashmap_get(&cx->tex_m, *path);
	if (found)
	{
		cleanup_str(path);
		*out_id = (*(t_index *)found - 1);
		return (SUCCESS);
	}
	init_default_texture(&new_tex);
	new_tex.path = *path;
	insert_index = vector_push_back(&cx->tex_v, &new_tex);
	if (insert_index < 0)
		return (ERR_MALLOC);
	*path = NULL;
	if (hashmap_insert(&cx->tex_m, new_tex.path, (void *)(intptr_t)(insert_index + 1)) < 0)
		return (ERR_MALLOC);
	*out_id = insert_index;
	return (SUCCESS);
}
