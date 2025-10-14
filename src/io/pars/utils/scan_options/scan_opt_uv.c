#include "io.h"
#include "type.h"
#include "error_codes.h"

static int	find_char_in_tok(t_tok tok, char c)
{
	int	index;

	index = 0;
	while (index < tok.len)
	{
		if (tok.start[index] == c)
			return (index);
		index++;
	}
	return (-1);
}

int	scan_opt_uv(t_tok tok, t_element_options *opts)
{
	int		sep;
	t_tok	a;
	t_tok	b;
	float	tmp[3];
	int		i;

	sep = find_char_in_tok(tok, ';');
	if (sep < 0)
	{
		if (scan_point(tok, tmp))
			return (ERR_PARSE_FLOAT);
		opts->uv[0] = tmp[0];
		opts->uv[1] = tmp[1];
		opts->uv[2] = tmp[2];
		opts->uv_mod = 1;
		return (SUCCESS);
	}
	a.start = tok.start;
	a.len = sep;
	b.start = tok.start + sep + 1;
	b.len = tok.len - sep - 1;
	if (scan_point(a, tmp))
		return (ERR_PARSE_FLOAT);
	i = 0;
	while (i < 3)
	{
		opts->uv[i] = tmp[i];
		i++;
	}
	if (scan_point(b, tmp))
		return (ERR_PARSE_FLOAT);
	i = 0;
	while (i < 3)
	{
		opts->uv[3 + i] = tmp[i];
		i++;
	}
	opts->uv_mod = 2;
	return (SUCCESS);
}
