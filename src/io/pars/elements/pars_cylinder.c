#include "io.h"
#include "type.h"
#include "errors.h"
#include "../parsing_internal.h"

int	pars_cylinder(t_pars_state *st, t_scene_parsed *scene)
{
	t_parsed_element	parsed_element;
	t_tok				token;
	int					status;

	init_parsed_element(&parsed_element);
	parsed_element.type = ELEM_CYLINDER;
	if (!pars_next_tok(st, &token)
		|| scan_point(token, parsed_element.data.cylinder.origin))
		return (ERR_PARS);
	if (!pars_next_tok(st, &token)
		|| scan_vec3(token, parsed_element.data.cylinder.axis))
		return (ERR_PARS);
	if (!pars_next_tok(st, &token)
		|| scan_float(token, &parsed_element.data.cylinder.diameter))
		return (ERR_PARS);
	if (!pars_next_tok(st, &token)
		|| scan_float(token, &parsed_element.data.cylinder.height))
		return (ERR_PARS);
	if (!pars_next_tok(st, &token)
		|| scan_color(token, parsed_element.rgb))
		return (ERR_PARS);
	status = pars_options(st, &parsed_element.options);
	if (status != SUCCESS)
		return (ERR_PARS);
	return (pars_register_element(scene, &parsed_element, ELEM_ROLE_NORMAL));
}
