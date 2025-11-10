NAME        := test_parsing
CC          := cc
CFLAGS      := -Wall -Wextra -Werror
INCLUDES    := -I../../includes
OBJ_DIR		:= obj

# === Libft ===
LIBFT_DIR   := ../../lib/libft
LIBFT       := $(LIBFT_DIR)/libft.a
INCLUDES    += -I$(LIBFT_DIR)/includes

# === Sources ===
# Prends uniquement le nouveau parsing, PAS le legacy
SRCS := \
	pars/elements/pars_ambient.c\
	pars/elements/pars_camera.c\
	pars/elements/pars_cylinder.c\
	pars/elements/pars_light.c\
	./pars/elements/pars_plane.c\
	./pars/elements/pars_resolution.c\
	./pars/elements/pars_sphere.c\
	./pars/elements/pars_triangle.c\
	./pars/pars_register/pars_register_element.c\
	./pars/utils/scan_options/scan_opt_ambient.c\
	./pars/utils/scan_options/scan_opt_diffuse.c\
	./pars/utils/scan_options/scan_opt_ior.c\
	./pars/utils/scan_options/scan_opt_reflection.c\
	./pars/utils/scan_options/scan_opt_refraction.c\
	./pars/utils/scan_options/scan_opt_shininess.c\
	./pars/utils/scan_options/scan_opt_specular.c\
	./pars/utils/scan_options/scan_option.c\
	./pars/utils/scan_options/scan_opt_uv.c\
	./pars/utils/scan_options/scan_opt_texture.c\
	./pars/utils/pars_option.c\
	./pars/utils/scan_color.c\
	./pars/utils/scan_float.c\
	./pars/utils/scan_point.c\
	./pars/utils/scan_vec3.c\
	./pars/convert/conv_camera.c\
	./pars/convert/conv_count_all.c\
	./pars/convert/conv_globals.c\
	./pars/convert/conv_light.c\
	./pars/convert/conv_option.c\
	./pars/convert/pars_convert_scene.c\
	./pars/convert/copy_primitive.c\
	./pars/convert/conv_primitive.c\
	./pars/convert/finalize.c\
	./pars/convert/options/generate_option_key.c\
	./pars/convert/options/intern_texture.c\
	./pars/convert/options/intern_material.c\
	./pars/convert/options/intern_surface.c\
	./pars/pars_ctx_init.c\
	./pars/pars_dispatch_tok.c\
	./pars/pars_init_scene.c\
	./pars/pars_line.c\
	./pars/pars_next_tok.c\
	./pars/pars_scene.c\
	./pars/pars_scene_fd.c\
	./pars/test_parsing.c\
	./pars/free_parsed_scene.c\
	./main_test_io.c\
	./scene_load_pipeline.c\
	./build/build_scene_from_ctx.c\
	./print_test_scene.c\
	../../engine/math/vector.c\
	../../engine/math/rsqrt.c\
	../../engine/io/rcp.c

OBJS := $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

.PHONY: all clean fclean re

all: $(LIBFT) $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) $(LIBFT) -lm -o $@

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)
	# optionnel : nettoyer la libft
	# $(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all
