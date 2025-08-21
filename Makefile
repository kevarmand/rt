# Header



# Header
SRC_DIR	:= ./src
INC_DIR	:= ./includes
OBJ_DIR := ./obj
LIB_DIR := ./lib


# Liste des sources
SRCS = pars/parser_obj.c pars/parser.c pars/parser_obj2.c pars/parser_utils.c cam_raytracing.c color.c UI/button.c UI/ui.c UI/close_ui.c UI/button_add.c UI/print_txt_ui.c UI/init.c UI/create_buttons/create_button.c UI/create_buttons/description_panel.c UI/create_buttons/tab_button.c render.c distance.c data.c init2.c test_mlx.c modif/translate.c modif/rotate.c modif/translate2.c modif/normalize.c modif/resize.c modif/rotate2.c rt.c lighting2.c utils/ft_itoa.c utils/ft_split.c utils/vector.c utils/vector2.c utils/ft_gnl.c utils/utils_list.c utils/ft_strjoin.c utils/equa.c utils/utils.c utils/utils2.c utils/vector3.c inter2.c handlers/mouse_move.c handlers/key_hook.c handlers/mouse_hook.c handlers/mouse_release.c error.c inter.c bmp.c init.c exit.c select.c lighting.c name_lst.c 
			
			
			
			
			
			
			
			
			
			
			
			
			

LIB		:= lib/libmlx.a

OBJ		:= $(addprefix $(OBJ_DIR)/,$(SRCS:.c=.o))

#Compilateur
CC		:= gcc
CFLAGS	:= -Wall -Wextra -O3
CFLAGS	+= -lm -lXext -lX11 -I $(INC_DIR)

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
	$(CC) -o $(NAME) $(OBJ) $(LIB) $(CFLAGS)

test:	$(NAME)
	./raytracer test3.rt

clean: 
	rm -rf $(OBJ_DIR)

fclean:	clean
	rm -f $(NAME)

re :	fclean all

.PHONY: all clean fclean re 

