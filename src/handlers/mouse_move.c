#include "rt.h"

int	mouse_move(int x, int y, t_data *data)
{
	if (data->ui.dragging)
	{
		int	new_x = x - data->ui.drag_offset_x;
		int	new_y = y - data->ui.drag_offset_y;
		int	max_x = data->para.res_width - WIDTH_UI;
		int	max_y = data->para.res_height - HEIGHT_UI;

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
