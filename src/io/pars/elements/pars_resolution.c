#include "io.h"
#include "type.h"
#include "error_codes.h"

int	pars_resolution(t_pars_state *st, t_scene_parsed *scene)
{
	t_tok	token;
	float	tmp;

	if (scene->presence_mask & PRESENCE_RESOLUTION)
		return (ERR_PARS);
	if (!pars_next_tok(st, &token) || scan_float(token, &tmp) || tmp <= 0.0f)
		return (ERR_PARS);
	scene->globals.res_width = (int)tmp;
	if (!pars_next_tok(st, &token) || scan_float(token, &tmp) || tmp <= 0.0f)
		return (ERR_PARS);
	scene->globals.res_height = (int)tmp;
	scene->presence_mask |= PRESENCE_RESOLUTION;
	return (SUCCESS);
}
