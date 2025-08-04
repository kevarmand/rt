#include "rt.h"

int	mouse_release(int button, int x, int y, t_data *data)
{
	(void)x;
	(void)y;
	if (button == 1 && data->ui.dragging)
	{
		data->ui.dragging = 0;
		data->flag_draw |= FLAG_DRAW_TEXT;
	}
	return (0);
}
