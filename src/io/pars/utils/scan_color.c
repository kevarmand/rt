#include "io.h"
#include "type.h"
#include "error_codes.h"


/* Lit un entier décimal 0..255, puis consomme une virgule si require_comma=1. */
static int	scan_uint8_component(
	t_tok tok, int *index_pos, int *value_out, int require_comma)
{
	int	value;
	int	digits_read;

	value = 0;
	digits_read = 0;
	while (*index_pos < tok.len
		&& tok.start[*index_pos] >= '0'
		&& tok.start[*index_pos] <= '9')
	{
		value = value * 10 + (tok.start[*index_pos] - '0');
		if (value > 255)
			return (ERR_PARSE_FLOAT);
		digits_read++;
		(*index_pos)++;
	}
	if (digits_read == 0)
		return (ERR_PARSE_FLOAT);
	if (require_comma)
	{
		if (*index_pos >= tok.len || tok.start[*index_pos] != ',')
			return (ERR_PARSE_FLOAT);
		(*index_pos)++;
	}
	*value_out = value;
	return (SUCCESS);
}
/**
 * @brief Parse a color from a token in the format "R,G,B".
 * @param tok      The input token containing the color. (no_null)
 * @param out_rgb  Output array to store the parsed RGB components. (no_null)
 * @return Returns 0 on success, or a non-zero error code on failure.
 * @note Each RGB component must be an integer in the range 0-255.
 */
int	scan_color(t_tok tok, int out_rgb[3])
{
	int	index_pos;

	index_pos = 0;
	if (scan_uint8_component(tok, &index_pos, &out_rgb[0], 1))
		return (ERR_PARSE_FLOAT);
	if (scan_uint8_component(tok, &index_pos, &out_rgb[1], 1))
		return (ERR_PARSE_FLOAT);
	if (scan_uint8_component(tok, &index_pos, &out_rgb[2], 0))
		return (ERR_PARSE_FLOAT);
	if (index_pos != tok.len)
		return (ERR_PARSE_FLOAT);
	return (SUCCESS);
}
