#include "io.h"
#include "type.h"
#include "error_codes.h"

int	scan_opt_ior(t_tok tok, t_element_options *opts)
{
	float	value;

	if (parse_float(tok, &value))
		return (ERR_PARS);
	if (value < 1.0f)
		return (ERR_PARS);
	opts->ior = value;
	return (SUCCESS);
}
