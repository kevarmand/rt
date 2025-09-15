#include "io.h"
#include "error_codes.h"

typedef int (*t_pars_handler)(t_pars_state *st, t_scene_parsed *scene);

typedef struct s_pars_entry {
	const char		*tag;
	t_pars_handler	fn;
}	t_pars_entry;

static const t_pars_entry g_pars_tab[] = {
	{"A",  pars_ambient},
	{"c",  pars_camera},
	{"l",  pars_light},
	{"sp", pars_sphere},
	{"pl", pars_plane},
	{"cy", pars_cylinder},
	{"tr", pars_triangle},
	{"sq", pars_square},
	{NULL, NULL}
};

int	pars_dispatch_tok(t_tok tag, t_pars_state *st, t_scene_parsed *scene)
{
	int i;

	i = 0;
	while (g_pars_tab[i].tag)
	{
		if (pars_tok_eq(tag, g_pars_tab[i].tag))
			return (g_pars_tab[i].fn(st, scene));
		i++;
	}
	return (ERR_PARS_TYPE);
}
