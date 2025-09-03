#include "rt.h"

void	print_txt_ui(t_data *data)
{
	mlx_string_put(data->mlx_ptr, data->win_ptr,
		data->ui.x + 10, data->ui.y + 20, UI_COLOR_TEXT, "UI Controls");
	print_all_button_txt(data);
}