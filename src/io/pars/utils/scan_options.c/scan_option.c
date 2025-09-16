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

static int	split_key_value(t_tok tok, t_tok *key_tok, t_tok *val_tok)
{
	int	index;

	index = 0;
	while (index < tok.len && tok.start[index] != '=')
		index++;
	if (index == 0 || index >= tok.len - 1)
		return (ERR_PARSE_FLOAT);
	key_tok->start = tok.start;
	key_tok->len = index;
	val_tok->start = tok.start + index + 1;
	val_tok->len = tok.len - index - 1;
	return (SUCCESS);
}

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
