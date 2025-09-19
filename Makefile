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

SRCS =  engine/render/buffer_to_mlx.c engine/render/tile_utils.c engine/render/render_begin.c engine/render/reset_tile.c engine/render/do_calculation.c engine/render/core/ren_dispatch.c engine/render/core/ren_init.c engine/render/core/ren_tiles_init.c engine/render/core/ren_cleanup.c engine/render/core/ren_tile_free.c engine/render/core/ren_tile_to_buffer.c engine/render/thread/thread.c engine/render/thread/worker_thread.c engine/render/render_tile.c engine/modif/normalize.c engine/modif/resize.c engine/modif/rotate.c engine/modif/rotate2.c engine/modif/translate.c engine/modif/translate2.c engine/tracer/ray_init.c engine/tracer/ray_trace.c legacy/bmp.c legacy/color.c legacy/data.c legacy/distance.c legacy/error.c legacy/exit.c legacy/init.c legacy/init2.c legacy/inter.c legacy/inter2.c legacy/lighting.c legacy/lighting2.c legacy/name_lst.c legacy/render.c legacy/rt.c legacy/select.c legacy/test_mlx.c legacy/utils/ft_gnl.c legacy/utils/ft_itoa.c legacy/utils/ft_split.c legacy/utils/ft_strjoin.c legacy/utils/utils.c legacy/utils/utils2.c legacy/utils/utils_list.c legacy/utils/vector.c legacy/utils/vector2.c legacy/utils/vector3.c legacy/utils/equa.c legacy/cam_raytracing.c app/tick/frame_tick.c app/tick/tile_tick.c app/handlers/key_hook.c app/handlers/mouse_hook.c app/handlers/mouse_move.c app/handlers/mouse_release.c app/UI/button.c app/UI/button_add.c app/UI/close_ui.c app/UI/create_buttons/create_button.c app/UI/create_buttons/description_panel.c app/UI/create_buttons/tab_button.c app/UI/init.c app/UI/print_txt_ui.c app/UI/ui.c vector_math/vector_sse.c io/pars/elements/pars_ambient.c io/pars/elements/pars_camera.c io/pars/elements/pars_cylinder.c io/pars/elements/pars_light.c io/pars/elements/pars_plane.c io/pars/elements/pars_resolution.c io/pars/elements/pars_sphere.c io/pars/elements/pars_triangle.c io/pars/legacy/parser.c io/pars/legacy/parser_obj.c io/pars/legacy/parser_obj2.c io/pars/legacy/parser_utils.c io/pars/pars_ctx_init.c io/pars/pars_dispatch_tok.c io/pars/pars_init_scene.c io/pars/pars_line.c io/pars/pars_next_tok.c io/pars/pars_register/pars_register_element.c io/pars/pars_scene.c io/pars/pars_scene_fd.c io/pars/utils/pars_option.c io/pars/utils/scan_color.c io/pars/utils/scan_float.c io/pars/utils/scan_point.c io/pars/utils/scan_vec3.c io/pars/utils/scan_options/scan_opt_ambient.c io/pars/utils/scan_options/scan_opt_diffuse.c io/pars/utils/scan_options/scan_opt_ior.c io/pars/utils/scan_options/scan_opt_reflection.c io/pars/utils/scan_options/scan_opt_refraction.c io/pars/utils/scan_options/scan_opt_shininess.c io/pars/utils/scan_options/scan_opt_specular.c io/pars/utils/scan_options/scan_option.c 



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
