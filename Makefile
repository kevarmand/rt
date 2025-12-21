# **************************************************************************** #
#                                    PROGRAM                                   #
# **************************************************************************** #

.DEFAULT_GOAL	:= all
NAME 			:= miniRT

# **************************************************************************** #
#                                     TOOLS                                    #
# **************************************************************************** #

CC			:= clang
CFLAGS		:= -g -Wall -Wextra -pthread -fno-math-errno -march=native -fenable-matrix   #  --static
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

MLX_DIR		:= lib/minilibx
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
    MLX_LINKS := -L$(MLX_DIR) -lmlx -lXext -lX11 -lm   #  -ldl -lxcb -lXau -lXdmcp -lXi
endif

LIBFT_LINKS := -L$(LIBFT_DIR) -lft

LDFLAGS     := $(MLX_LINKS) $(LIBFT_LINKS)

# **************************************************************************** #
#                                    SOURCES                                   #
# **************************************************************************** #
# NB: les chemins ci-dessous sont RELATIFS à $(SRCS_DIR), comme dans ton message.

SRCS = app/display/buffer_to_mlx.c app/display/display_init2.c app/display/display_init.c app/loop/frame_tick.c app/UI/draw_ui.c app/UI/build_ui.c app/UI/init_ui.c app/UI/ui.c app/UI/ui_cam_status.c app/UI/ui_print.c app/camera/camera_dolly.c app/camera/camera_level.c app/camera/camera_look.c app/camera/camera_math.c app/camera/camera_orbit.c app/camera/camera_pan.c app/camera/camera_roll.c app/camera/update_camera.c app/input/mouse_hook.c app/input/mouse_move.c app/input/mouse_release.c app/input/key_hook.c app/resources/xpm_albedo.c app/resources/xpm_bump.c app/resources/xpm_helper.c app/resources/xpm_load.c app/sync/sync_engine_tick.c app/sync/sync_helper.c app/sync/sync_policy.c app/sync/sync_send.c app/sync/sync_receive.c app/run_app.c engine/math/vec3f_math2.c engine/math/vec3f_math3.c engine/math/equation_solvers.c engine/math/solve_quartic.c engine/math/rsqrt.c engine/math/matrix.c engine/math/vec3f_math.c engine/math/rcp.c engine/math/order.c engine/intersect/scene_hit.c engine/intersect/scene_is_occluded.c engine/bvh/sah.c engine/bvh/sah_bins.c engine/bvh/torus_inter.c engine/bvh/aabb.c engine/bvh/aabb_logic.c engine/bvh/aabb_logic0.c engine/bvh/bvh.c engine/bvh/bvh_build.c engine/bvh/prim_inter.c engine/geometry/primitives/cylinder.c engine/geometry/primitives/plane.c engine/geometry/primitives/sphere.c engine/geometry/primitives/triangle.c engine/geometry/primitives/torus.c engine/geometry/utils/geometry_bump.c engine/geometry/utils/geometry_common.c engine/geometry/utils/geometry_tbn.c engine/geometry/utils/geometry_uv.c engine/geometry/utils/geometry_uv_prim.c engine/geometry/utils/torus_utils.c engine/geometry/hit_build_geometry.c engine/init/init_cam_view.c engine/init/init_surface_uv.c engine/init/init_surface_uv2.c engine/init/engine_init.c engine/ray/ray_finalize.c engine/render/init/render_init.c engine/render/init/render_init2.c engine/render/init/render_destroy.c engine/render/manager/convert_hdr_to_rgb.c engine/render/manager/core/init_worker_tile.c engine/render/manager/core/ren_dispatch.c engine/render/manager/manager_assign_jobs.c engine/render/manager/manager_collect_tiles.c engine/render/manager/read_mailbox.c engine/render/manager/tonemap/manager_run_tonemap.c engine/render/manager/tonemap/tonemap_apply.c engine/render/manager/tonemap/tonemap_hist.c engine/render/manager/tonemap/tonemap_hist_build.c engine/render/manager/tonemap/tonemap_hist_core.c engine/render/manager/update_display.c engine/render/manager/view_setup.c engine/render/manager/manager_entry.c engine/render/thread/thread_worker.c engine/render/thread/thread_join.c engine/render/thread/thread_manager.c engine/render/thread/thread_start.c engine/render/worker/core/build_ray_for_pixel.c engine/render/worker/core/render_fast.c engine/render/worker/core/render_fast2.c engine/render/worker/core/render_tile.c engine/render/worker/core/render_super.c engine/render/worker/worker_entry.c engine/shading/hit/fresnel_schlick.c engine/shading/hit/shade_refraction.c engine/shading/hit/shade_reflection.c engine/shading/hit/shade_ambient.c engine/shading/hit/shade_direct.c engine/shading/hit/shade_hit.c engine/shading/skybox/sky_color.c engine/shading/skybox/skybox_eval.c engine/shading/skybox/skybox_utils.c engine/shading/surface/apply_surface_shading.c engine/shading/utils/sample_texture.c engine/shading/shading_ray.c io/build/convert/options/generate_option_key.c io/build/convert/options/intern_material.c io/build/convert/options/intern_surface.c io/build/convert/conv_build_ctx.c io/build/convert/conv_cameras.c io/build/convert/conv_count_all.c io/build/convert/conv_globals.c io/build/convert/conv_lights.c io/build/convert/conv_option.c io/build/convert/conv_primitives.c io/build/convert/copy_primitive.c io/build/assemble_scene.c io/build/build_scene.c io/build/build_texture.c io/build/conv_ctx_lifetime.c io/parse/elements/pars_ambient.c io/parse/elements/pars_camera.c io/parse/elements/pars_cylinder.c io/parse/elements/pars_default_option.c io/parse/elements/pars_light.c io/parse/elements/pars_plane.c io/parse/elements/pars_resolution.c io/parse/elements/pars_skybox.c io/parse/elements/pars_sphere.c io/parse/elements/pars_torus.c io/parse/elements/pars_triangle.c io/parse/pars_dispatch_tok.c io/parse/pars_line.c io/parse/pars_next_tok.c io/parse/pars_register/pars_register_element.c io/parse/pars_scene.c io/parse/scan/pars_option.c io/parse/scan/scan_color.c io/parse/scan/scan_float.c io/parse/scan/scan_options/scan_opt_ambient.c io/parse/scan/scan_options/scan_opt_bscale.c io/parse/scan/scan_options/scan_opt_checkerboard.c io/parse/scan/scan_options/scan_opt_diffuse.c io/parse/scan/scan_options/scan_opt_ior.c io/parse/scan/scan_options/scan_opt_reflection.c io/parse/scan/scan_options/scan_opt_refraction.c io/parse/scan/scan_options/scan_opt_shininess.c io/parse/scan/scan_options/scan_opt_specular.c io/parse/scan/scan_options/scan_opt_texture.c io/parse/scan/scan_options/scan_opt_uv.c io/parse/scan/scan_options/scan_option.c io/parse/scan/scan_point.c io/parse/scan/scan_vec3.c io/load_scene.c io/parsed_scene_lifetime.c utils/check_args.c utils/color/color.c utils/error.c utils/bitmap.c utils/bitmap2.c utils/logs/logs_info_parsed/log_info_frame.c utils/logs/logs_info_parsed/log_info_layout.c utils/logs/logs_info_parsed/log_info_lines.c utils/logs/logs_info_parsed/log_info_parsed.c utils/logs/logs_info_parsed/log_info_utils_len.c utils/logs/logs_info_parsed/log_utils.c utils/logs/logs_steps0.c utils/logs/logs_steps1.c utils/logs/logs_steps.c main.c 

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
