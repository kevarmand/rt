#include "io.h"
#include "type.h"
#include "error_codes.h"

int	pars_plane(t_pars_state *st, t_scene_parsed *scene)
{
	t_parsed_element	parsed_element;
	t_tok				token;
	int					status;

	ft_bzero(&parsed_element, sizeof(parsed_element));
	parsed_element.type = ELEM_PLANE;
	if (!pars_next_tok(st, &token) || scan_point(token,
		parsed_element.data.plane.origin))
		return (ERR_PARS_TYPE);
	if (!pars_next_tok(st, &token) || scan_vec3(token,
		parsed_element.data.plane.normal))
		return (ERR_PARS_TYPE);
	if (!pars_next_tok(st, &token) || scan_color(token,
		parsed_element.data.plane.rgb))
		return (ERR_PARS_TYPE);
	status = pars_options(st, &parsed_element.options);
	if (status != 0)
		return (ERR_PARS_TYPE);
	return (pars_register_element(scene, &parsed_element, ELEM_ROLE_NORMAL));
}

