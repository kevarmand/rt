#include "io.h"
#include "type.h"
#include "error_codes.h"

typedef int	(*t_opt_handler)(t_tok, t_element_options *);

typedef struct s_opt_entry {
	const char		*key;
	t_opt_handler	fn;
}	t_opt_entry;

static const t_opt_entry	g_opt_tab[] = {
	{"ior",          scan_opt_ior},
	{"refraction",   scan_opt_refraction},
	{"reflection",   scan_opt_reflection},
	{"shininess",    scan_opt_shininess},
	{"diffuse",      scan_opt_diffuse},
	{"specular",     scan_opt_specular},
	{"ambient",      scan_opt_ambient},
	{NULL, NULL}
};

/**
 * @brief Split a token of the form "key=value" into key and value tokens.
 * @param tok        The input token to split. (no_null)
 * @param key_out    Output token for the key part. (no_null)
 * @param value_out  Output token for the value part. (no_null)
 * @return 0 on success, or ERR_PARSE_FLOAT if the format is invalid.
 * @note Both output tokens point into the original token's string.
 */
int	scan_option(t_tok tok, t_element_options *opts)
{
	t_tok	key_tok;
	t_tok	val_tok;
	int		table_index;

	if (split_key_value(tok, &key_tok, &val_tok))
		return (ERR_PARSE_FLOAT);
	table_index = 0;
	while (g_opt_tab[table_index].key)
	{
		if (pars_tok_eq(key_tok, g_opt_tab[table_index].key))
			return (g_opt_tab[table_index].fn(val_tok, opts));
		table_index++;
	}
	return (ERR_PARS_TYPE);
}
