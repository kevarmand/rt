#include "rt.h"
#include <string.h>

void	add_axis_labels(t_ui *ui, int start_x, int start_y)
{
	static const char	*axes[3] = { " Ox", " Oy", " Oz" };
	int					i;

	i = 0;
	while (i < 3)
	{
		add_button(ui, (t_button){
			.x = start_x, .y = start_y + i * 20,
			.width = 32, .height = 16,
			.label = 0, .txt = strdup(axes[i]),
			.color_bg = UI_COLOR_FOND, .color_txt = UI_COLOR_TEXT,
			.on_click = NULL
		});
		i++;
	}
}

void	add_axis_signs(t_ui *ui, int base_x, int base_y, void (*on_click)(void *, int))
{
	int	i;

	for (i = 0; i < 3; i++)
	{
		add_button(ui, (t_button){
			.x = base_x + 140, .y = base_y + i * 20,
			.width = 16, .height = 16,
			.label = '-', .txt = NULL,
			.color_bg = 0xFF0000, .color_txt = 0x000000,
			.on_click = on_click
		});
		add_button(ui, (t_button){
			.x = base_x + 160, .y = base_y + i * 20,
			.width = 16, .height = 16,
			.label = '+', .txt = NULL,
			.color_bg = 0x00FF00, .color_txt = 0x000000,
			.on_click = on_click
		});
	}
}

void	create_buttons(t_ui *ui, t_data *data)
{
	//bouton exit
	add_button(ui, (t_button){
		.x = UI_EXIT_X, .y = UI_EXIT_Y,
		.width = UI_EXIT_W, .height = UI_EXIT_H,
		.label = 'X', .txt = NULL,
		.color_bg = UI_COLOR_EXIT, .color_txt = UI_COLOR_TEXT,
		.on_click = close_ui
	});
	//barre de titre
	add_button(ui, (t_button){
		.x = 0, .y = 0,
		.width = WIDTH_UI, .height = UI_HEADER_HEIGHT,
		.label = 0, .txt = NULL,
		.color_bg = UI_COLOR_FOND, .color_txt = UI_COLOR_TEXT,
		.on_click = NULL
	});
	add_button(ui, (t_button){
		.x = 60, .y = 40, .width = 70, .height = 16,
		.label = 0,
		.txt = (data->select.obj->name),
		.color_bg = UI_COLOR_ACCENT, .color_txt = UI_COLOR_TEXT,
		.on_click = NULL,
	});
	add_button(ui, (t_button){
		.x = 10, .y = 60, .width = 100, .height = 16,
		.label = 0, .txt = "color",
		.color_bg = UI_COLOR_FOND, .color_txt = UI_COLOR_TEXT,
		.on_click = NULL
	});
	add_button(ui, (t_button){
		.x = 10, .y = 100, .width = 180, .height = 16,
		.label = 0, .txt = UI_LABEL_TRANSLATION,
		.color_bg = UI_COLOR_FOND, .color_txt = UI_COLOR_TEXT,
		.on_click = NULL
	});
	add_button(ui, (t_button){
		.x = 10, .y = 220, .width = 180, .height = 16,
		.label = 0, .txt = UI_LABEL_ROTATION,
		.color_bg = UI_COLOR_FOND, .color_txt = UI_COLOR_TEXT,
		.on_click = NULL,
	});
	add_axis_labels(ui, 10, 130);
	add_axis_labels(ui, 10, 250);
	printf("Creating buttons for UI\n");
	//afficher data->ui->butons_count
	printf("Number of buttons created: %d\n", ui->button_count);

	add_axis_signs(ui, 10, 130, NULL);
	add_axis_signs(ui, 10, 250, NULL);
}
