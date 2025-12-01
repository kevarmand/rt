#include "io.h"
#include "type.h"
#include "errors.h"
#include "../../parsing_internal.h"

int	scan_opt_ior(t_tok tok, t_element_options *opts)
{
	float	value;

	if (scan_float(tok, &value))
		return (ERR_PARS);
	if (value < 0.0f)
		return (ERR_PARS);
	opts->ior = value;
	return (SUCCESS);
}
