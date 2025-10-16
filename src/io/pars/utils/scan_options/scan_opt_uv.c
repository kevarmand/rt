#include "io.h"
#include "type.h"
#include "error_codes.h"

static int	read_f32_segment(
	t_tok tok, int *index_pos, float *out_value, int require_comma)
{
	t_tok	seg;
	int		start;

	start = *index_pos;
	while (*index_pos < tok.len && tok.start[*index_pos] != ',')
		(*index_pos)++;
	seg.start = tok.start + start;
	seg.len = *index_pos - start;
	if (seg.len <= 0 || scan_float(seg, out_value))
		return (ERR_PARSE_FLOAT);
	if (require_comma)
	{
		if (*index_pos >= tok.len || tok.start[*index_pos] != ',')
			return (ERR_PARSE_FLOAT);
		(*index_pos)++;
	}
	return (SUCCESS);
}

static int	copy_floats_to_opts(
	t_element_options *opts, float *src, int count, int *uv_mod)
{
	int	i;

	if (count == 3)
		*uv_mod = 1;
	else if (count == 6)
		*uv_mod = 2;
	else
		return (ERR_PARSE_FLOAT);
	i = 0;
	while (i < count)
	{
		opts->uv[i] = src[i];
		i++;
	}
	return (SUCCESS);
}

int	scan_opt_uv(t_tok tok, t_element_options *opts)
{
	int		index;
	int		pos;
	float	tmp[6];

	pos = 0;
	index = 0;
	while (index < 6)
	{
		if (read_f32_segment(tok, &pos, &tmp[index], 0))
			break ;
		index++;
		if (pos == tok.len)
			break ;
		if (tok.start[pos] != ',')
			return (ERR_PARSE_FLOAT);
		pos++;
	}
	return (copy_floats_to_opts(opts, tmp, index, &opts->uv_mod));
}
