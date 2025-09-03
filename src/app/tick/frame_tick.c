
#include "rt.h"
#include "ui.h"

int	 frame_tick(t_data *data)
{
	if (data->render.camera)// on a une camera, si une fois les calcul fini on met cam = NULL
	render_tick(data);
	if (data->flag_draw & FLAG_DRAW_RESET_UI)
	{
		reset_ui(data);
	}
	if (data->flag_draw & FLAG_DRAW_UI)
	{
		mlx_put_image_to_window(data->mlx_ptr, data->win_ptr,
			(data->view)->img_ptr, 0, 0);
	}
	if (data->ui.visible && data->flag_draw & FLAG_DRAW_UI)
	{
		// on affichge l'interface utilisateur
		printf("UI is visible at position (%d, %d)\n", data->ui.x, data->ui.y);
		// on affiche ladresse de limage :
		printf("UI image address: %p\n", data->ui.img.addr_ptr);
		mlx_put_image_to_window(data->mlx_ptr, data->win_ptr,
		(data->ui).img.img_ptr, (data->ui).x, (data->ui).y);
	}
	if (data->ui.visible && data->flag_draw & FLAG_DRAW_TEXT)
	{
		print_txt_ui(data);
	}
	data->flag_draw = 0;
	return (0);
}