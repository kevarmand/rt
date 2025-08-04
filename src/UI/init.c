#include "rt.h"

void put_pixel_to_image(t_image *img, int x, int y, int color)
{
	char	*dst;
	// On vérifie que les coordonnées sont dans les limites de l'image
	if (x < 0 || x >= WIDTH_UI || y < 0 || y >= HEIGHT_UI)
		return;
		
	// On récupère l'adresse du pixel à modifier
	dst = img->addr_ptr + (y * img->size_l + x * (img->bpp / 8));
	// On place la couleur dans le pixel
	*(unsigned int *)(dst) = color;
}
/***
 * @brief Initializes the UI for the mlx window.
 * 
 * This function sets up the UI structure, creates a new image for the UI,
 * 
 * @param data Pointer to the t_data structure containing UI and mlx information.
 * @note: This function assumes that the mlx_ptr in data is already initialized.
 * It also assumes that the WIDTH_UI and HEIGHT_UI constants are defined.
 * If the image creation fails, it will print an error message and exit the program.
 */
void	setup_ui_mlx(t_data *data)
{
	data->ui.visible = 0;
	data->ui.dragging = 0;
	data->ui.x = 50;
	data->ui.y = 50;
	data->ui.img.img_ptr = mlx_new_image(data->mlx_ptr, WIDTH_UI, HEIGHT_UI);
	if (!data->ui.img.img_ptr)
	{
		fprintf(stderr, "Error: Failed to create UI image.\n");
		ft_exit(data);
	}
	data->ui.img.addr_ptr = mlx_get_data_addr(data->ui.img.img_ptr,
		&data->ui.img.bpp, &data->ui.img.size_l, &data->ui.img.endian);
	data->ui.button_count = 0;
}


/***
 * @brief Draws a rectangle on the given image.
 * 
 * TODO :MESAGE DERREUR
 * 
 * @param img Pointer to the image where the rectangle will be drawn.
 * @param rect Pointer to the rectangle structure containing position, size, and color.
 * @return int Returns SUCCESS if the rectangle is drawn successfully, otherwise returns ERROR.
 */
int	draw_rectangle(t_image *img, t_rect *rect)
{
	int	x;
	int	y;

	if (!img || !rect)
		return (ERROR);
	// On vérifie que les dimensions du rectangle sont valides
	if (rect->width <= 0 || rect->height <= 0)
		return (ERROR);
	if (rect->x < 0 || rect->y < 0 || rect->x + rect->width > WIDTH_UI || rect->y + rect->height > HEIGHT_UI)
		return (ERROR);
	y = 0;
	while (y < rect->height)
	{
		x = 0;
		while (x < rect->width)
		{
			put_pixel_to_image(img, rect->x + x, rect->y + y, rect->color);
			x++;
		}
		y++;
	}
	return (SUCCESS);
}


/***
 * @brief Creates the UI interface by drawing a rectangle.
 * 
 */
void	create_interface(t_data *data)
{
	draw_rectangle(&data->ui.img, &(t_rect){UI_COLOR_FOND,
		WIDTH_UI, HEIGHT_UI, 0, 0});
	draw_rectangle(&data->ui.img, &(t_rect){UI_COLOR_EXIT,
		UI_EXIT_W, UI_EXIT_H, UI_EXIT_X, UI_EXIT_Y});
	draw_rectangle(&data->ui.img, &(t_rect){UI_COLOR_BORDER,
		WIDTH_UI - 2 * UI_CONTENT_X, HEIGHT_UI - UI_HEADER_HEIGHT - UI_CONTENT_X, UI_CONTENT_X, UI_HEADER_HEIGHT});
	draw_rectangle(&data->ui.img, &(t_rect){UI_COLOR_FIELD_BG,
		WIDTH_UI - 4 * UI_CONTENT_X, 95, 2 * UI_CONTENT_X, 100});
	draw_rectangle(&data->ui.img, &(t_rect){UI_COLOR_FIELD_BG,
		WIDTH_UI - 4 * UI_CONTENT_X, 95, 2 * UI_CONTENT_X, 220});
	// Ajouter les 3 champ ici
}

void	init_ui(t_data *data)
{
	setup_ui_mlx(data);
	create_interface(data);
	create_buttons(&data->ui, data);
	draw_all_buttons(&data->ui);
	draw_escape_cross(&data->ui.img);
	data->ui.visible = 0;
}
