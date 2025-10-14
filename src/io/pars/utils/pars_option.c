#include "io.h"
#include "type.h"
#include "error_codes.h"

/***
 * Parse options for an element from the token stream.
 * @param st      The current parsing state. (no_null)
 * @param options Pointer to the element options structure to populate. (no_null)
 * @return Returns 0 on success, or a non-zero error code on failure.
 * @note The function reads tokens until no more valid options are found.
 * Each option is expected to be in the format "key=value".
 */
int	pars_options(t_pars_state *st, t_element_options *options)
{
	t_tok	token;
	int		ret;

	while (pars_next_tok(st, &token))
	{
		ret = scan_option(token, options);
		if (ret != SUCCESS)
			return (ret);
	}
	return (SUCCESS);
}