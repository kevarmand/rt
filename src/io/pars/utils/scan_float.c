#include "io.h"
#include "type.h"
#include "errors.h"
#include "../parsing_internal.h"

static const double	g_max_abs = 1e10;

static void	scan_sign(t_tok tok, int *index_pos, int *sign_out)
{
	*sign_out = 1;
	if (*index_pos < tok.len)
	{
		if (tok.start[*index_pos] == '+'
			|| tok.start[*index_pos] == '-')
		{
			if (tok.start[*index_pos] == '-')
				*sign_out = -1;
			(*index_pos)++;
		}
	}
}

static int	scan_integer(t_tok tok, int *index_pos, double *value_io)
{
	int		digits_read;

	digits_read = 0;
	while (*index_pos < tok.len
		&& tok.start[*index_pos] >= '0'
		&& tok.start[*index_pos] <= '9')
	{
		*value_io = (*value_io * 10.0)
			+ (double)(tok.start[*index_pos] - '0');
		digits_read++;
		(*index_pos)++;
		if (*value_io > g_max_abs)
			return (-1);
	}
	return (digits_read);
}

static int	scan_fraction(t_tok tok, int *index_pos, double *value_io)
{
	int		digits_read;
	double	frac_scale;
	double	tmp;

	digits_read = 0;
	frac_scale = 0.1f;
	while (*index_pos < tok.len
		&& tok.start[*index_pos] >= '0'
		&& tok.start[*index_pos] <= '9')
	{
		tmp = ((double)(tok.start[*index_pos] - '0')) * (frac_scale);
		(*value_io) += tmp;
		digits_read++;
		frac_scale *= 0.1f;
		(*index_pos)++;
	}
	return (digits_read);
}

static int	scan_optional_dot_and_fraction(
	t_tok tok, int *index_pos, double *value_io)
{
	int	digits;

	digits = 0;
	if (*index_pos < tok.len && tok.start[*index_pos] == '.')
	{
		(*index_pos)++;
		digits = scan_fraction(tok, index_pos, value_io);
	}
	return (digits);
}

int	scan_float(t_tok tok, float *out_value)
{
	int		index_pos;
	int		sign;
	int		int_digits;
	int		frac_digits;
	double	value;

	index_pos = 0;
	value = 0.0;
	scan_sign(tok, &index_pos, &sign);
	int_digits = scan_integer(tok, &index_pos, &value);
	if (int_digits < 0)
		return (ERR_PARSE_FLOAT);
	frac_digits = scan_optional_dot_and_fraction(tok, &index_pos, &value);
	if ((int_digits + frac_digits) == 0 || index_pos != tok.len)
		return (ERR_PARSE_FLOAT);
	value *= (double)sign;
	if (value > g_max_abs || value < -g_max_abs)
		return (ERR_PARSE_FLOAT);
	*out_value = (float)value;
	return (SUCCESS);
}
