#include "rt.h"
/***
 * create_descr - Function to create the description panel buttons.
 * buttons are : name, color and next button.
 */
void    create_descr(t_ui *ui, t_data *data)
{
	//cree le fond du pannel
	add_button(ui, (t_button){
		.x = UI_DESCRIPTION_X, .y = UI_DESCRIPTION_Y,
		.width = UI_DESCRIPTION_W, .height = UI_DESCRIPTION_H,
		.label = 0, .txt = NULL,
		.color_bg = UI_COLOR_PANEL, .color_txt = NULL,
		.on_click = NULL
	});
	//creer le bouton name
	add_button(ui, (t_button){
		.x = UI_BUTTON_NAME_X + UI_DESCRIPTION_X,
		.y = UI_BUTTON_NAME_Y + UI_DESCRIPTION_Y,
		.width = UI_BUTTON_NAME_W, .height = UI_BUTTON_NAME_H,
		.label = 0, .txt = UI_LABEL_NAME,
		.color_bg = UI_COLOR_PANEL, .color_txt = UI_COLOR_TEXT,
		.on_click = NULL
	});

	//creer le couton color (couleur du fond//texte color)
	//grise si lobjet selectionne n a pas de couleur ( cam par ex)
	add_button(ui, (t_button){
		.x = UI_BUTTON_COLOR_X + UI_DESCRIPTION_X,
		.y = UI_BUTTON_COLOR_Y + UI_DESCRIPTION_Y,
		.width = UI_BUTTON_COLOR_W, .height = UI_BUTTON_COLOR_H,
		.label = 0, .txt = "Color :", //le texte sera la couleur de lobjet
		.color_bg = UI_COLOR_PANEL, .color_txt = UI_COLOR_TEXT,
		.on_click = NULL
	});
	//creer el boutoncolor avec en fond la couleur de lobjet (ecrire sr la couoleur complementaire ? ca peut etre drole)
	//grise si lobjet selectionne n a pas de couleur ( cam par ex)
	add_button(ui, (t_button){
		.x = UI_BUTTON_NEXT_X + UI_DESCRIPTION_X,
		.y = UI_BUTTON_NEXT_Y + UI_DESCRIPTION_Y,
		.width = UI_BUTTON_NEXT_W, .height = UI_BUTTON_NEXT_H,
		.label = 0, .txt = UI_LABEL_NEXT,
		.color_bg = UI_COLOR_ACCENT, .color_txt = UI_COLOR_TEXT,
		.on_click = NULL
	});
}
