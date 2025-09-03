#include "rt.h"

int	mouse_drag_ui(int rel_x, int rel_y, t_data *data)
{
	data->ui.dragging = 1;
	data->ui.drag_offset_x = rel_x;
	data->ui.drag_offset_y = rel_y;
	return (1);
}

int	mouse_click_ui(int rel_x, int rel_y, t_data *data)
{
	t_ui		*ui = &(data->ui);
	t_button	*btn;
	int			i;

	i = ui->button_count;
	while (--i >= 0)
	{
		btn = &ui->buttons[i];
		if (rel_x >= btn->x && rel_x < btn->x + btn->width &&
			rel_y >= btn->y && rel_y < btn->y + btn->height)
		{
			if  (i  == 0)
				return (mouse_drag_ui(rel_x, rel_y, data));
			if (btn->on_click)
			{
				btn->on_click(data);
				printf("Button[%i] clicked: %s\n", i, btn->txt ? btn->txt : "No text");
				return (1); // bouton trouvé et action exécutée
			}
		}

	}
	return (0); // aucun bouton cliqué
}

int	mouse_press(int button, int x, int y, t_data *data)
{
	int	rel_x;
	int	rel_y;
	if (button == 1 && data->ui.visible)
	{
		rel_x = x - data->ui.x;
		rel_y = y - data->ui.y;
		if (mouse_click_ui(rel_x, rel_y, data))
			data->flag_draw |= FLAG_DRAW_UI | FLAG_DRAW_TEXT;
	}
	return (0);
}