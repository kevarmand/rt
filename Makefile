# Header



# Header
SRC_DIR	:= ./src
INC_DIR	:= ./includes
OBJ_DIR := ./obj
LIB_DIR := ./lib


# Liste des sources
SRCS = engine/render/buffer_to_mlx.c engine/render/tile_utils.c engine/render/render_tile.c engine/render/render_begin.c engine/render/reset_tile.c engine/render/do_calculation.c engine/render/core/ren_dispatch.c engine/render/core/ren_init.c engine/render/core/ren_tiles_init.c engine/render/core/ren_cleanup.c engine/render/core/ren_tile_free.c engine/render/core/ren_tile_to_buffer.c engine/render/thread/thread.c engine/render/thread/worker_thread.c engine/modif/normalize.c engine/modif/resize.c engine/modif/rotate.c engine/modif/rotate2.c engine/modif/translate.c engine/modif/translate2.c legacy/bmp.c legacy/cam_raytracing.c legacy/color.c legacy/data.c legacy/distance.c legacy/error.c legacy/exit.c legacy/init.c legacy/init2.c legacy/inter.c legacy/inter2.c legacy/lighting.c legacy/lighting2.c legacy/name_lst.c legacy/render.c legacy/rt.c legacy/select.c legacy/test_mlx.c legacy/utils/equa.c legacy/utils/ft_gnl.c legacy/utils/ft_itoa.c legacy/utils/ft_split.c legacy/utils/ft_strjoin.c legacy/utils/utils.c legacy/utils/utils2.c legacy/utils/utils_list.c legacy/utils/vector.c legacy/utils/vector2.c legacy/utils/vector3.c app/tick/frame_tick.c app/tick/tile_tick.c app/handlers/key_hook.c app/handlers/mouse_hook.c app/handlers/mouse_move.c app/handlers/mouse_release.c app/UI/button.c app/UI/button_add.c app/UI/close_ui.c app/UI/create_buttons/create_button.c app/UI/create_buttons/description_panel.c app/UI/create_buttons/tab_button.c app/UI/init.c app/UI/print_txt_ui.c app/UI/ui.c io/pars/parser.c io/pars/parser_obj.c io/pars/parser_obj2.c io/pars/parser_utils.c 
			
			
			
			
			
			
			
			
			
			
			
			
			

LIB		:= lib/libmlx.a

OBJ		:= $(addprefix $(OBJ_DIR)/,$(SRCS:.c=.o))

#Compilateur
CC		:= cc
CFLAGS	:= -Wall -Wextra -O3
CFLAGS	+= -I $(INC_DIR)
LDFLAGS := -L $(LIB_DIR) -lmlx -lm -lXext -lX11


NAME := miniRT



# Regles :
all:	$(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
	mkdir -p $(OBJ_DIR)/pars
	mkdir -p $(OBJ_DIR)/utils
	mkdir -p $(OBJ_DIR)/modif

$(OBJ_DIR)/%.o:$(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ -c $<

$(NAME): $(OBJ_DIR) $(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(LIB) $(CFLAGS) $(LDFLAGS)

test:	$(NAME)
	./raytracer test3.rt

clean: 
	rm -rf $(OBJ_DIR)

fclean:	clean
	rm -f $(NAME)

re :	fclean all

.PHONY: all clean fclean re 

