#include "io.h"

int	pars_next_tok(t_pars_state *st, t_tok *out)
{
	int	i;

	i = st->pos;
	while (st->line[i] == ' ' || st->line[i] == '\t')
		i++;
	if (st->line[i] == '\0' || st->line[i] == '\n'
		|| st->line[i] == '\r' || st->line[i] == '#')
	{
		st->pos = i;
		return (0);
	}
	out->start = st->line + i;
	while (st->line[i] && st->line[i] != ' ' && st->line[i] != '\t'
		&& st->line[i] != '\n' && st->line[i] != '\r')
		i++;
	out->len = i - (int)(out->start - st->line);
	st->pos = i;
	return (1);
}

int	pars_tok_eq(t_tok t, const char *lit)
{
	int i;

	i = 0;
	while (i < t.len && lit[i] && t.start[i] == lit[i])
		i++;
	return (i == t.len && lit[i] == '\0');
}
