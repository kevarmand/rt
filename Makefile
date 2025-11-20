# **************************************************************************** #
#                                    PROGRAM                                   #
# **************************************************************************** #

.DEFAULT_GOAL	:= all
NAME 			:= miniRT

# **************************************************************************** #
#                                     TOOLS                                    #
# **************************************************************************** #

CC			:= cc
CFLAGS		:= -Wall -Wextra -pthread -ffast-math -fno-math-errno -Ofast -march=native
DEP_FLAGS	:= -MMD -MP
AR			:= ar rcs
RM			:= rm -rf

# **************************************************************************** #
#                                  DIRECTORIES                                 #
# **************************************************************************** #

SRCS_DIR	:= src
INCLD_DIR	:= includes
OBJS_DIR	:= objs

LIBFT_DIR	:= lib/libft
LIBFT_A		:= $(LIBFT_DIR)/libft.a

MLX_DIR		:= lib/minilibx            # ← adapte-moi (ex: mlx_linux / minilibx-linux)
MLX_A		:= $(MLX_DIR)/libmlx.a

# **************************************************************************** #
#                                     INCLUDES                                 #
# **************************************************************************** #

INC_DIRS    := $(INCLD_DIR) $(LIBFT_DIR) $(MLX_DIR)
INCLD_FLAG  := $(addprefix -I,$(INC_DIRS))

# **************************************************************************** #
#                                  PLATFORM/MLX                                #
# **************************************************************************** #

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)                 # macOS
    MLX_LINKS := -L$(MLX_DIR) -lmlx -framework OpenGL -framework AppKit
else                                     # Linux / WSL
    MLX_LINKS := -L$(MLX_DIR) -lmlx -lXext -lX11 -lm
endif

LIBFT_LINKS := -L$(LIBFT_DIR) -lft

LDFLAGS     := $(MLX_LINKS) $(LIBFT_LINKS)

# **************************************************************************** #
#                                    SOURCES                                   #
# **************************************************************************** #
# NB: les chemins ci-dessous sont RELATIFS à $(SRCS_DIR), comme dans ton message.

SRCS = \
			io/pars/elements/pars_ambient.c \
			io/pars/elements/pars_camera.c \
			io/pars/elements/pars_cylinder.c \
			io/pars/elements/pars_light.c \
			io/pars/elements/pars_plane.c \
			io/pars/elements/pars_resolution.c \
			io/pars/elements/pars_sphere.c \
			io/pars/elements/pars_triangle.c \
			io/pars/pars_register/pars_register_element.c \
			io/pars/utils/scan_options/scan_opt_ambient.c \
			io/pars/utils/scan_options/scan_opt_diffuse.c \
			io/pars/utils/scan_options/scan_opt_ior.c \
			io/pars/utils/scan_options/scan_opt_reflection.c \
			io/pars/utils/scan_options/scan_opt_refraction.c \
			io/pars/utils/scan_options/scan_opt_shininess.c \
			io/pars/utils/scan_options/scan_opt_specular.c \
			io/pars/utils/scan_options/scan_option.c \
			io/pars/utils/scan_options/scan_opt_uv.c \
			io/pars/utils/scan_options/scan_opt_texture.c \
			io/pars/utils/pars_option.c \
			io/pars/utils/scan_color.c \
			io/pars/utils/scan_float.c \
			io/pars/utils/scan_point.c \
			io/pars/utils/scan_vec3.c \
			io/pars/pars_state_init.c \
			io/pars/pars_dispatch_tok.c \
			io/pars/pars_init_scene.c \
			io/pars/pars_line.c \
			io/pars/pars_next_tok.c \
			io/pars/pars_scene.c \
			io/pars/test_parsing.c \
			io/pars/free_parsed_scene.c \
			io/main_test_io.c \
			io/build/convert/conv_camera.c \
			io/build/convert/conv_count_all.c \
			io/build/convert/conv_globals.c \
			io/build/convert/conv_light.c \
			io/build/convert/conv_option.c \
			io/build/convert/pars_convert_scene.c \
			io/build/convert/copy_primitive.c \
			io/build/convert/conv_primitive.c \
			io/build/convert/finalize.c \
			io/build/convert/options/generate_option_key.c \
			io/build/convert/options/intern_texture.c \
			io/build/convert/options/intern_material.c \
			io/build/convert/options/intern_surface.c \
			io/load_scene.c \
			color/color.c \
			error.c \
			check_args.c \
			main.c \
			print_test_scene.c


# **************************************************************************** #
#                                    OBJECTS                                   #
# **************************************************************************** #

OBJS := $(patsubst %.c,$(OBJS_DIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

# **************************************************************************** #
#                                     RULES                                    #
# **************************************************************************** #

all: $(NAME)

# Build libs first
$(LIBFT_A):
	$(MAKE) -C $(LIBFT_DIR)

$(MLX_A):
	$(MAKE) -C $(MLX_DIR)

# Link program
$(NAME): $(LIBFT_A) $(MLX_A) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(INCLD_FLAG) $(LDFLAGS) -o $(NAME)

# Pattern rule for objects (+ auto mkdir)
-include $(DEPS)
$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(DEP_FLAGS) $(CFLAGS) $(INCLD_FLAG) -c $< -o $@

# Convenience
run: $(NAME)
	./$(NAME) cat.rt

clean:
	$(RM) $(OBJS_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean
	-$(MAKE) -C $(MLX_DIR) clean

fclean: clean
	$(RM) $(NAME)
	$(RM) $(LIBFT_A)

re: fclean all

.PHONY: all clean fclean re run
