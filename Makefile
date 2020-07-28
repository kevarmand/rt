# Header



# Header
SRC_DIR	:= ./src
INC_DIR	:= ./includes
OBJ_DIR := ./obj
LIB_DIR := ./lib


# Liste des sources
SRC		:=	cam_raytracing.c\
			color.c\
			create_obj.c\
			data.c\
			distance.c\
			equa.c\
			error.c\
			ft_gnl.c\
			ft_split.c\
			inter.c\
			lighting.c\
			parser.c\
			parser_obj.c\
			parser_utils.c\
			rt.c\
			test_mlx.c\
			test_parse.c\
			transf.c\
			utils.c\
			utils_list.c\
			vector.c

LIB		:= libmlx.a

OBJ		:= $(addprefix $(OBJ_DIR)/,$(SRC:.c=.o))

#Compilateur
CC		:= gcc
CFLAGS	:=
CFLAGS	+= -lm -lXext -lX11 -I $(INC_DIR)

NAME := raytracer



# Regles :
all:	$(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o:$(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(NAME): $(OBJ_DIR) $(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(LIB) $(CFLAGS)
	./raytracer test2.rt

clean: 
	rm -rf $(OBJ_DIR)

fclean:	clean
	rm -f $(NAME)

re :	fclean all

.PHONY: all clean fclean re 

