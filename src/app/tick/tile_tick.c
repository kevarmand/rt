#include "render.h"
#include "rt.h"

int	render_tick(t_data *data)
{

	//     int      x0, y0;

//     tile_index_to_pos(tile->id, data->para.res_width, &x0, &y0);
	//verifier si on a des tuile a rendre
	int i;
	int modified = 0;

	i = 0;
	while (i < NB_CORE - 1)
	{
		if (atomic_load(&data->render.tiles[i].is_done) == 1)
		{
			if  (data->render.tiles[i].id != -1)
			{
				modified = 1;
				ren_tile_to_buffer(data->render.framebuffer, &data->render.tiles[i],
					data->render.width);
				(data->render.tiles_done)++;
			}
			data->render.tiles[i].id = ren_dispatch(data->render.tiles_bitmap,
			data->render.tiles_total, &data->render.tiles_distributed);
			if (data->render.tiles[i].id != -1)
				atomic_store(&data->render.tiles[i].is_done, 0);
		}
		i++;
	}
	if (modified)
	{
		buffer_to_mlx_image(data->view, data->render.framebuffer,
			data->render.width);
		mlx_put_image_to_window(data->mlx_ptr, data->win_ptr,
			(data->view)->img_ptr, 0, 0);
	}
	if (NB_CORE == 1 )
		do_calculations(data);
	if (data->render.tiles_done >= data->render.tiles_total)
	{
		printf("Render complete\n");
		// pour l'instant on fait mais on enchainera sur le calcule des
		// caméras suivantes
		// data->render.cam = get_next_cam(data);
		// if (data->render.cam)
		// 	render_begin(data);
		//exit(0);
		data->render.camera = NULL; // on a fini le rendu
	}
	return (SUCCESS);
}