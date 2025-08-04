
#ifndef UI_H
# define UI_H

# include "type.h"

# define WIDTH_UI 200
# define HEIGHT_UI 450


# define UI_COLOR_FOND 0x0A64AD
# define UI_COLOR_EXIT 0xFF2020
# define UI_COLOR_BORDER 0xF9E7D0
# define UI_COLOR_ACCENT 0x00FF00
# define UI_COLOR_TEXT 0xFFFFFF

# define UI_EXIT_W 20
# define UI_EXIT_H 20
# define UI_EXIT_X 175
# define UI_EXIT_Y 5

# define UI_HEADER_HEIGHT 30
# define UI_COLOR_FIELD_BG 0xDDDDDD
# define UI_CONTENT_X 5

//Button parameters
#define UI_LABEL_OBJECT      "Object"
#define UI_LABEL_NAME        "Name"
#define UI_LABEL_TRANSLATION "        Translation"
#define UI_LABEL_ROTATION    "          Rotation"

void	init_ui(t_data *data);
void	print_txt_ui(t_data *data);
int		add_button(t_ui *ui, t_button btn);
void	draw_all_buttons(t_ui *ui);
void	print_all_button_txt(t_data *data);
void	create_interface(t_data *data);
int		draw_rectangle(t_image *img, t_rect *rect);
void	create_buttons(t_ui *ui, t_data *data);
void	draw_escape_cross(t_image *img);
void	put_pixel_to_image(t_image *img, int x, int y, int color);
void	close_ui(t_data *data, int flag);

#endif