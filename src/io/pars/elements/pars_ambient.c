#include "io.h"
#include "type.h"
#include "errors.h"
#include "../parsing_internal.h"

int	pars_ambient(t_pars_state *st, t_scene_parsed *scene)
{
	t_tok	token;
	float	brightness;
	int		rgb[3];

	if (scene->presence_mask & PRESENCE_AMBIENT)
		return (ERR_PARS_AMBIANT);
	if (!pars_next_tok(st, &token)
		|| scan_float(token, &brightness)
		|| brightness < 0.0f || brightness > 1.0f)
		return (ERR_PARS_AMBIANT);
	if (!pars_next_tok(st, &token)
		|| scan_color(token, rgb))
		return (ERR_PARS);
	scene->globals.brightness = brightness;
	scene->globals.color[0] = (float)rgb[0];
	scene->globals.color[1] = (float)rgb[1];
	scene->globals.color[2] = (float)rgb[2];
	scene->presence_mask |= PRESENCE_AMBIENT;
	return (SUCCESS);
}
