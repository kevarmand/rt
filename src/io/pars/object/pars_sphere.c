#include "io.h"
#include "type.h"
#include "error_codes.h"


int	pars_sphere(t_pars_state *st, t_scene_parsed *scene)
{
	t_parsed_element	e;
	t_tok				t;

	ft_bzero(&e, sizeof(e));
	e.type = ELEM_SPHERE;
	if (!pars_next_tok(st, &t)
		|| pars_parse_vec3_tok(t, (t_vec3*)&e.data.sphere.center))
		return (ERR_SP_POS);
	if (!pars_next_tok(st, &t) || pars_scan_double_tok(t, (double*)&e.data.sphere.diameter)
		|| e.data.sphere.diameter <= 0.0)
		return (ERR_SP_DIAM);
	if (!pars_next_tok(st, &t) || pars_parse_color_tok(t, (t_rgb*)&e.data.sphere.rgb))
		return (ERR_SP_COLOR);
	while (pars_next_tok(st, &t))
		if (pars_option_tok(t, &e.options))
			return (ERR_OPT_VALUE);
	return (pars_builder_push_object(scene, &e, &st->ctx[st->depth]));
}
