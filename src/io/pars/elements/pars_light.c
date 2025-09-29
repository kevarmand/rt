#include "io.h"
#include "type.h"
#include "error_codes.h"

int	pars_light(t_pars_state *st, t_scene_parsed *scene, t_elem_role role)
{
	t_parsed_element	parsed_element;
	t_tok				token;
	int					status;

	ft_bzero(&parsed_element, sizeof(parsed_element));
	parsed_element.type = ELEM_LIGHT;
	if (!pars_next_tok(st, &token) || scan_point(token,
		parsed_element.data.light.position))
		return (ERR_PARS);
	if (!pars_next_tok(st, &token) || scan_float(token,
		&parsed_element.data.light.brightness))
		return (ERR_PARS);
	if (parsed_element.data.light.brightness < 0.0f
		|| parsed_element.data.light.brightness > 1.0f)
		return (ERR_PARS);
	if (!pars_next_tok(st, &token) || scan_color(token,
		parsed_element.data.light.rgb))
		return (ERR_PARS);
	status = pars_options(st, &parsed_element.options);
	if (status != 0)
		return (ERR_PARS);
	return (pars_register_element(scene, &parsed_element, role));
}
