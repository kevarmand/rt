#include "io.h"
#include "type.h"
#include "error_codes.h"

int	pars_line(const char *line, t_scene_parsed *scene, t_pars_state *st)
{
	t_tok	tag;

	st->line = line;
	st->pos = 0;
	if (!pars_next_tok(st, &tag))
		return (SUCCESS);
	return (pars_dispatch_tok(tag, st, scene));
}