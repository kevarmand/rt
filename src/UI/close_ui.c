#include "rt.h"

void	close_ui(t_data *data)
{
	if (data->ui.visible)
	{
		data->ui.visible = 0;
		data->flag_draw |= FLAG_DRAW_UI;
	}
}