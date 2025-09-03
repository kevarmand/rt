#include "rt.h"

int	mouse_move(int x, int y, t_data *data)
{
	int new_x;
	int new_y;
	int max_x;
	int max_y;

	if (data->ui.dragging)
	{
		new_x = x - data->ui.drag_offset_x;
		new_y = y - data->ui.drag_offset_y;
		max_x = data->para.res_width - WIDTH_UI;
		max_y = data->para.res_height - HEIGHT_UI;
		if (new_x < 0)
			new_x = 0;
		else if (new_x > max_x)
			new_x = max_x;
		if (new_y < 0)
			new_y = 0;
		else if (new_y > max_y)
			new_y = max_y;
		data->ui.x = new_x;
		data->ui.y = new_y;
		data->flag_draw |= FLAG_DRAW_UI;
	}
	return (0);
}
