#include "io.h"
#include "type.h"
#include "errors.h"
#include "../../parsing_internal.h"

static char	*dup_tok_str(t_tok tok)
{
	char	*out;
	int		i;

	out = (char *)malloc((size_t)tok.len + 1);
	if (!out)
		return (NULL);
	i = 0;
	while (i < tok.len)
	{
		out[i] = tok.start[i];
		i++;
	}
	out[i] = '\0';
	return (out);
}

int	scan_opt_texture(t_tok tok, t_element_options *opts)
{
	char	*path;

	path = dup_tok_str(tok);
	if (!path)
		return (ERR_PARS);
	opts->texture_path = path;
	return (SUCCESS);
}

int	scan_opt_bump(t_tok tok, t_element_options *opts)
{
	char	*path;

	path = dup_tok_str(tok);
	if (!path)
		return (ERR_PARS);
	opts->bumpmap_path = path;
	return (SUCCESS);
}
