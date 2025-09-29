NAME        := test_parsing
CC          := cc
CFLAGS      := -Wall -Wextra -Werror
INCLUDES    := -I../../../includes

# === Libft ===
LIBFT_DIR   := ../../../lib/libft
LIBFT       := $(LIBFT_DIR)/libft.a
INCLUDES    += -I$(LIBFT_DIR)/includes

# === Sources ===
# Prends uniquement le nouveau parsing, PAS le legacy
SRCS := \
	./pars_ctx_init.c \
	./pars_dispatch_tok.c \
	./pars_init_scene.c \
	./pars_line.c \
	./pars_next_tok.c \
	./pars_scene.c \
	./pars_scene_fd.c \
	./test_parsing.c \
	./elements/pars_ambient.c \
	./elements/pars_camera.c \
	./elements/pars_cylinder.c \
	./elements/pars_light.c \
	./elements/pars_plane.c \
	./elements/pars_resolution.c \
	./elements/pars_sphere.c \
	./elements/pars_triangle.c \
	./pars_register/pars_register_element.c \
	./utils/pars_option.c \
	./utils/scan_color.c \
	./utils/scan_float.c \
	./utils/scan_point.c \
	./utils/scan_vec3.c \
	./utils/scan_options/scan_opt_ambient.c \
	./utils/scan_options/scan_opt_diffuse.c \
	./utils/scan_options/scan_option.c \
	./utils/scan_options/scan_opt_ior.c \
	./utils/scan_options/scan_opt_reflection.c \
	./utils/scan_options/scan_opt_refraction.c \
	./utils/scan_options/scan_opt_shininess.c \
	./utils/scan_options/scan_opt_specular.c

OBJS := $(SRCS:.c=.o)

.PHONY: all clean fclean re

all: $(LIBFT) $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) $(LIBFT) -lm -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)
	# optionnel : nettoyer la libft
	# $(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all
