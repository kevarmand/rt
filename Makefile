# Header



# Header
SRC_DIR	:= ./src
INC_DIR	:= ./includes
OBJ_DIR := ./obj
LIB_DIR := ./lib


# Liste des sources
SRCS = \
			bmp.c \
			cam_raytracing.c \
			color.c \
			data.c \
			distance.c \
			error.c \
			exit.c \
			handlers/key_hook.c \
			handlers/mouse_hook.c \
			handlers/mouse_move.c \
			handlers/mouse_release.c \
			init2.c \
			init.c \
			inter2.c \
			inter.c \
			lighting2.c \
			lighting.c \
			modif/normalize.c \
			modif/resize.c \
			modif/rotate2.c \
			modif/rotate.c \
			modif/translate2.c \
			modif/translate.c \
			name_lst.c \
			pars/parser.c \
			pars/parser_obj2.c \
			pars/parser_obj.c \
			pars/parser_utils.c \
			render.c \
			rt.c \
			select.c \
			test_mlx.c \
			UI/button_add.c \
			UI/button.c \
			UI/close_ui.c \
			UI/create_button.c \
			UI/init.c \
			UI/print_txt_ui.c \
			UI/ui.c \
			utils/equa.c \
			utils/ft_gnl.c \
			utils/ft_itoa.c \
			utils/ft_split.c \
			utils/ft_strjoin.c \
			utils/utils2.c \
			utils/utils.c \
			utils/utils_list.c \
			utils/vector2.c \
			utils/vector3.c \
			utils/vector.c \
			
			
			
			
			
			
			
			
			
			
			
			
			

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

