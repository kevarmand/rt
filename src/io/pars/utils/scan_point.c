#include "io.h"
#include "type.h"
#include "error_codes.h"

static int	read_component(
	t_tok tok, int *index_pos, float *value_out, int require_comma)
{
	t_tok	segment;
	int		start_idx;

	start_idx = *index_pos;
	while (*index_pos < tok.len && tok.start[*index_pos] != ',')
		(*index_pos)++;
	segment.start = tok.start + start_idx;
	segment.len = *index_pos - start_idx;
	if (segment.len <= 0 || scan_float(segment, value_out))
		return (ERR_PARSE_FLOAT);
	if (require_comma)
	{
		if (*index_pos >= tok.len || tok.start[*index_pos] != ',')
			return (ERR_PARSE_FLOAT);
		(*index_pos)++;
	}
	return (SUCCESS);
}

/***
 * @brief Scan a 3D point from a token span.
 * @param tok The token span containing the point representation. (no_null)
 * @param out_vec Pointer to store the parsed 3D point (array of 3 floats).
 *  (no_null)
 * @return SUCCESS on successful parse, or ERR_PARSE_FLOAT on failure.
 * @note The function expects the point to be in the format "x,y,z"
 * where x, y, and z are floating-point numbers. 
 * It does not handle spaces or other delimiters.
 */
int	scan_point(t_tok tok, float out_vec[3])
{
	int		index_pos;
	float	tmp[3];

	index_pos = 0;
	if (read_component(tok, &index_pos, &tmp[0], 1))
		return (ERR_PARSE_FLOAT);
	if (read_component(tok, &index_pos, &tmp[1], 1))
		return (ERR_PARSE_FLOAT);
	if (read_component(tok, &index_pos, &tmp[2], 0))
		return (ERR_PARSE_FLOAT);
	if (index_pos != tok.len)
		return (ERR_PARSE_FLOAT);
	out_vec[0] = tmp[0];
	out_vec[1] = tmp[1];
	out_vec[2] = tmp[2];
	return (SUCCESS);
}

