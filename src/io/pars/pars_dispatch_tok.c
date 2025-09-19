#include "io.h"
#include "error_codes.h"

typedef int	(*t_pars_handler)(t_pars_state *st, t_scene_parsed *scene);

static int	handle_camera_primary(t_pars_state *st, t_scene_parsed *scene)
{
	return (pars_camera(st, scene, ELEM_ROLE_PRIMARY));
}

static int	handle_camera_extra(t_pars_state *st, t_scene_parsed *scene)
{
	return (pars_camera(st, scene, ELEM_ROLE_EXTRA));
}

static int	handle_light_primary(t_pars_state *st, t_scene_parsed *scene)
{
	return (pars_light(st, scene, ELEM_ROLE_PRIMARY));
}

static int	handle_light_extra(t_pars_state *st, t_scene_parsed *scene)
{
	return (pars_light(st, scene, ELEM_ROLE_EXTRA));
}

typedef struct s_pars_entry {
	const char		*tag;
	t_pars_handler	fn;
}	t_pars_entry;

static const t_pars_entry g_pars_tab[] = {
	{"A", pars_ambient},
	{"R", pars_resolution},
	{"C", handle_camera_primary},
	{"c", handle_camera_extra},
	{"L", handle_light_primary},
	{"l", handle_light_extra},
	{"sp", pars_sphere},
	{"tr", pars_triangle},
	{"pl", pars_plane},
	{"cy", pars_cylinder},
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
	return (ERR_PARS);
}
