#include "rt.h"

void	close_ui(t_data *data, int flag)
{
	(void)flag;
	if (data->ui.visible)
	{
		data->ui.visible = 0;
		data->flag_draw |= FLAG_DRAW_UI;
	}
}