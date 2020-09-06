# Header



# Header
SRC_DIR	:= ./src
INC_DIR	:= ./includes
OBJ_DIR := ./obj
LIB_DIR := ./lib


# Liste des sources
SRC		:=	bmp.c\
			cam_raytracing.c\
			color.c\
			data.c\
			distance.c\
			utils/equa.c\
			error.c\
			exit.c\
			utils/ft_gnl.c\
			utils/ft_split.c\
			utils/ft_strjoin.c\
			inter.c\
			inter2.c\
			key_hook.c\
			lighting.c\
			lighting2.c\
			modif/normalize.c\
			pars/parser.c\
			pars/parser_obj.c\
			pars/parser_obj2.c\
			pars/parser_utils.c\
			render.c\
			modif/resize.c\
			modif/rotate.c\
			modif/rotate2.c\
			rt.c\
			select.c\
			test_mlx.c\
			init.c\
			init2.c\
			modif/translate.c\
			modif/translate2.c\
			utils/utils.c\
			utils/utils2.c\
			utils/utils_list.c\
			utils/vector.c\
			utils/vector2.c\
			utils/vector3.c

LIB		:= lib/libmlx.a

OBJ		:= $(addprefix $(OBJ_DIR)/,$(SRC:.c=.o))

#Compilateur
CC		:= gcc
CFLAGS	:= -Wall -Wextra -Werror
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

