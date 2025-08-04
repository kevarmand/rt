#include "rt.h"

int	mouse_press(int button, int x, int y, t_data *data)
{
	if (button == 1)
	{
		int rel_x = x - data->ui.x;
		int rel_y = y - data->ui.y;
		if (rel_x >= 0 && rel_x < WIDTH_UI &&
			rel_y >= 0 && rel_y < UI_HEADER_HEIGHT)
		{
			data->ui.dragging = 1;
			data->ui.drag_offset_x = rel_x;
			data->ui.drag_offset_y = rel_y;
		}
	}
	return (0);
}