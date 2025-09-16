#include "io.h"
#include "type.h"
#include "error_codes.h"

int	pars_resolution(t_pars_state *st, t_scene_parsed *scene)
{
	t_tok	token;
	int		width;
	int		height;

	if (scene->presence_mask & PRESENCE_RESOLUTION)
		return (ERR_PARS_TYPE);
	if (!pars_next_tok(st, &token)
		|| scan_int(token, &width) || width <= 0)
		return (ERR_PARS_TYPE);
	if (!pars_next_tok(st, &token)
		|| scan_int(token, &height) || height <= 0)
		return (ERR_PARS_TYPE);
	scene->globals.res_width = width;
	scene->globals.res_height = height;
	scene->presence_mask |= PRESENCE_RESOLUTION;
	return (0);
}
