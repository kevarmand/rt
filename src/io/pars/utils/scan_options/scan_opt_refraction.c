#include "io.h"
#include "type.h"
#include "error_codes.h"

int	scan_opt_refraction(t_tok tok, t_element_options *opts)
{
	float	value;

	if (scan_float(tok, &value))
		return (ERR_PARSE_FLOAT);
	if (value < 1.0f)
		return (ERR_PARSE_FLOAT);
	opts->refraction = value;
	return (0);
}
