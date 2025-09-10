# **************************************************************************** #
#                                    PROGRAM                                   #
# **************************************************************************** #

.DEFAULT_GOAL	:= all
NAME 			:= miniRT

# **************************************************************************** #
#                                     TOOLS                                    #
# **************************************************************************** #

CC			:= cc
CFLAGS		:= -Wall -Wextra -O3 -g
DEP_FLAGS	:= -MMD -MP
AR			:= ar rcs
RM			:= rm -rf

# **************************************************************************** #
#                                  DIRECTORIES                                 #
# **************************************************************************** #

SRCS_DIR	:= src
INCLD_DIR	:= includes
OBJS_DIR	:= objs

LIBFT_DIR	:= libft
LIBFT_A		:= $(LIBFT_DIR)/libft.a

MLX_DIR		:= minilibx-linux            # ← adapte-moi (ex: mlx_linux / minilibx-linux)
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

SRCS =  engine/render/buffer_to_mlx.c \
        engine/render/tile_utils.c \
        engine/render/render_tile.c \
        engine/render/render_begin.c \
        engine/render/reset_tile.c \
        engine/render/do_calculation.c \
        engine/render/core/ren_dispatch.c \
        engine/render/core/ren_init.c \
        engine/render/core/ren_tiles_init.c \
        engine/render/core/ren_cleanup.c \
        engine/render/core/ren_tile_free.c \
        engine/render/core/ren_tile_to_buffer.c \
        engine/render/thread/thread.c \
        engine/render/thread/worker_thread.c \
        engine/modif/normalize.c \
        engine/modif/resize.c \
        engine/modif/rotate.c \
        engine/modif/rotate2.c \
        engine/modif/translate.c \
        engine/modif/translate2.c \
        legacy/bmp.c \
        legacy/cam_raytracing.c \
        legacy/color.c \
        legacy/data.c \
        legacy/distance.c \
        legacy/error.c \
        legacy/exit.c \
        legacy/init.c \
        legacy/init2.c \
        legacy/inter.c \
        legacy/inter2.c \
        legacy/lighting.c \
        legacy/lighting2.c \
        legacy/name_lst.c \
        legacy/render.c \
        legacy/rt.c \
        legacy/select.c \
        legacy/test_mlx.c \
        legacy/utils/equa.c \
        legacy/utils/ft_gnl.c \
        legacy/utils/ft_itoa.c \
        legacy/utils/ft_split.c \
        legacy/utils/ft_strjoin.c \
        legacy/utils/utils.c \
        legacy/utils/utils2.c \
        legacy/utils/utils_list.c \
        legacy/utils/vector.c \
        legacy/utils/vector2.c \
        legacy/utils/vector3.c \
        app/tick/frame_tick.c \
        app/tick/tile_tick.c \
        app/handlers/key_hook.c \
        app/handlers/mouse_hook.c \
        app/handlers/mouse_move.c \
        app/handlers/mouse_release.c \
        app/UI/button.c \
        app/UI/button_add.c \
        app/UI/close_ui.c \
        app/UI/create_buttons/create_button.c \
        app/UI/create_buttons/description_panel.c \
        app/UI/create_buttons/tab_button.c \
        app/UI/init.c \
        app/UI/print_txt_ui.c \
        app/UI/ui.c \
        io/pars/parser.c \
        io/pars/parser_obj.c \
        io/pars/parser_obj2.c \
        io/pars/parser_utils.c

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
	./$(NAME) test3.rt

clean:
	$(RM) $(OBJS_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean
	-$(MAKE) -C $(MLX_DIR) clean

fclean: clean
	$(RM) $(NAME)
	$(RM) $(LIBFT_A)

re: fclean all

.PHONY: all clean fclean re run
