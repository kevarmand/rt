#include "rt.h"
#include "render.h"

void	render_begin(t_data *data, t_cam *cam)
{
	int	i;

	data->render.camera = cam;
	i = 0;
	while (i < NB_CORE - 1)
	{
		data->render.tiles[i].camera = cam;
		i++;
	}
	if (NB_CORE == 1)
		data->render.tiles[0].camera = cam;
}
