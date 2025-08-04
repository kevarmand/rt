#include "rt.h"

void	draw_plus_sign(t_image *img, t_button *btn)
{
	draw_rectangle(img, &(t_rect){
		0xFFFFFF, 2, 12, 7 + btn->x, 2 + btn->y });
	draw_rectangle(img, &(t_rect){
		0xFFFFFF, 12, 2, 2 + btn->x, 7 + btn->y });

}


void	draw_minus_sign(t_image *img, t_button *btn)
{
	draw_rectangle(img, &(t_rect){
		0xFFFFFF, 12, 2, 2 + btn->x, 7 + btn->y });
}

void	draw_escape_cross(t_image *img)
{
	int	i;

	i = 4;
	while (i < UI_EXIT_W - 4)
	{
		put_pixel_to_image(img, UI_EXIT_X + i, UI_EXIT_Y + i, 0xFFFFFF);
		put_pixel_to_image(img, UI_EXIT_X + i, UI_EXIT_Y + i + 1, 0xFFFFFF);
		put_pixel_to_image(img, UI_EXIT_X + (UI_EXIT_W - 1 - i), UI_EXIT_Y + i, 0xFFFFFF);
		put_pixel_to_image(img, UI_EXIT_X + (UI_EXIT_W - 1 - i), UI_EXIT_Y + i + 1, 0xFFFFFF);
		i++;
	}
}


void	draw_button(t_image *img, t_button *btn)
{
	draw_rectangle(img, &(t_rect){
		btn->color_bg, btn->width, btn->height, btn->x, btn->y });
	 if (btn->label == '+')
		draw_plus_sign(img, btn);
	if (btn->label == '-')
		draw_minus_sign(img, btn);
}

void	draw_all_buttons(t_ui *ui)
{
	int i;

	i = ui->button_count;
	while (--i >= 0)
	{
		draw_button(&(ui->img), &(ui->buttons[i]));
	}
}

void	print_button_txt(t_data *data, t_button *btn)
{
	//on put str le tesste de btn->txt
	if (!btn || !btn->txt)
		return;
	mlx_string_put(data->mlx_ptr, data->win_ptr,
		data->ui.x + btn->x + 3, data->ui.y + btn->y + 12, btn->color_txt, btn->txt);
}

void	print_all_button_txt(t_data *data)
{
	int i;
	
	i = 0;
	while (i < data->ui.button_count)
	{
		print_button_txt(data, &(data->ui.buttons[i]));
		i++;
	}
}