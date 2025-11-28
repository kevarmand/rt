#include "rt.h"


void	select_objet_tab(t_data *data)
{
	if (data->select.active_tab == UI_TAB_OBJECT)
		return ;
	data->select.active_tab = UI_TAB_OBJECT;
	data->flag_draw = FLAG_DRAW_UI | FLAG_DRAW_TEXT | FLAG_DRAW_RESET_UI;
	data->ui.visible = 1;
}

void	select_light_tab(t_data *data)
{
	if (data->select.active_tab == UI_TAB_LIGHT)
		return ;
	data->select.active_tab = UI_TAB_LIGHT;
	data->flag_draw = FLAG_DRAW_UI | FLAG_DRAW_TEXT | FLAG_DRAW_RESET_UI;
	data->ui.visible = 1;
}

void	select_camera_tab(t_data *data)
{
	if (data->select.active_tab == UI_TAB_CAMERA)
		return ;
	data->select.active_tab = UI_TAB_CAMERA;
	data->flag_draw = FLAG_DRAW_UI | FLAG_DRAW_TEXT | FLAG_DRAW_RESET_UI;
	data->ui.visible = 1;
}

static const void (*g_select_tab[])(t_data *) = {
	select_objet_tab,
	select_light_tab,
	select_camera_tab
};

void	create_buttons_tab(t_ui *ui, t_data *data)
{
	static const char *tabs[3] = { "Object", " Light", "Camera" };
	static const t_button template = {
		.x = UI_TAB_BUTTON_X, .y = UI_TAB_BUTTON_Y,
		.width = UI_TAB_BUTTON_W, .height = UI_TAB_BUTTON_H,
		.label = 0, .txt = NULL,
		.color_bg = UI_COLOR_INACTIVE, .color_txt = UI_COLOR_TEST_MUTED,
		.on_click = NULL
	};
	t_button	btn;
	int			i;
	
	i = 0;
	while (i < 3)
	{
		btn = template;
		btn.txt = tabs[i];
		btn.x = UI_TAB_BUTTON_X + i * (UI_TAB_BUTTON_W);
		if (i == data->select.active_tab)
			btn.color_bg = UI_COLOR_FIELD_ACTIVE;
		if (i == data->select.active_tab)
			btn.color_txt = UI_COLOR_TEXT;
		btn.on_click = g_select_tab[i];
		if (add_button(ui, btn) == ERROR)
		{
			fprintf(stderr, "Error adding button %s\n", tabs[i]);
			return;
		}
		i++;
	}
}

/***
 * @brief Creates the field description for the UI.
 * 
 * This field contains :
 * name, type color of the object,
 * 
 * @param ui Pointer to the t_ui structure containing UI information.
 * @param data Pointer to the t_data structure containing application data.
 */
void    create_tab(t_ui *ui, t_data *data)
{
	create_buttons_tab(ui, data);
	add_button(ui, (t_button){
		.x = UI_PANNEL_X, .y = UI_PANNEL_Y,
		.width = UI_PANNEL_W, .height = UI_PANNEL_H,
		.label = 0, .txt = NULL,
		.color_bg = UI_COLOR_FIELD_BG, .color_txt = NULL,
		.on_click = NULL
	});
/*
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
	*/
}