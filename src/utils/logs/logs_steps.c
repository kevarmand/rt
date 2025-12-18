/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logs_steps.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 23:37:57 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/17 14:11:20 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "logs.h"
#include "libft.h"

static void	print_clear(void)
{
	ft_putstr_fd("\033[2J\033[H", 1);
}

static void	print_cyan_header(void)
{
	ft_putstr_fd("\033[1;36m\n", 1);
	ft_putstr_fd("╔══════════════════════════════════════════════════════════════════════════════╗\n", 1);
	ft_putstr_fd("║                                                                              ║\n", 1);
	ft_putstr_fd("║                ███╗   ███╗██╗███╗   ██╗██╗██████╗ ████████╗                  ║\n", 1);
	ft_putstr_fd("║                ████╗ ████║██║████╗  ██║██║██╔══██╗╚══██╔══╝                  ║\n", 1);
	ft_putstr_fd("║                ██╔████╔██║██║██╔██╗ ██║██║██████╔╝   ██║                     ║\n", 1);
	ft_putstr_fd("║                ██║╚██╔╝██║██║██║╚██╗██║██║██╔══██╗   ██║                     ║\n", 1);
	ft_putstr_fd("║                ██║ ╚═╝ ██║██║██║ ╚████║██║██║  ██║   ██║                     ║\n", 1);
	ft_putstr_fd("║                ╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝╚═╝  ╚═╝   ╚═╝                     ║\n", 1);
	ft_putstr_fd("║                                                                              ║\n", 1);
	ft_putstr_fd("║                          by kearmand - norivier                              ║\n", 1);
	ft_putstr_fd("║                                                                              ║\n", 1);
	ft_putstr_fd("╚══════════════════════════════════════════════════════════════════════════════╝\n", 1);
	ft_putstr_fd("\033[0m\n", 1);
}

static void	put_blue_pipe(void)
{
	ft_putstr_fd("  \033[1;34m│\033[1;33m", 1);
}

static void	print_line_features(const char *logo,
						const char *label,
						const char *value)
{
	ft_putstr_fd("  \033[1;34m│\033[1;33m  " , 1);
	ft_putstr_fd(logo, 1);
	ft_putstr_fd("  ", 1);
	ft_putstr_fd(label, 1);
	ft_putstr_fd(" : \033[1;37m", 1);
	ft_putstr_fd(value, 1);
	ft_putstr_fd("\033[1;33m\n", 1);
}

static void	print_geometry_lines(void)
{
	ft_putstr_fd("  \033[1;34m│\033[1;33m  🧬  Geometry :  \033[1;37m🟠 Sphere     🟦 Plane\033[1;33m\n", 1);
	ft_putstr_fd("  \033[1;34m│\033[1;33m                  \033[1;37m🍩 Torus      🛢️ Cylinder\033[1;33m\n", 1);
	ft_putstr_fd("  \033[1;34m│\033[1;33m                        \033[1;37m🔺 Triangle   (Möller–Trumbore)\033[1;33m\n", 1);
}

static void	print_features_box(void)
{
	ft_putstr_fd("\033[1;34m", 1);
	ft_putstr_fd("  ┌──────────────────────────────────────────────────────────────┐\n", 1);
	print_line_features("🧠", "Shading ", "Phong / Specular / Ambient");
	print_line_features("💡", "Lights  ", "Colored lights");
	print_line_features("🌑", "Shadows ", "Hard shadows");
	print_line_features("🪞", "Optics  ", "Reflection / Refraction");
	print_line_features("🧱", "Surfaces", "Texture / Checkerboard / Bump mapping");
	print_line_features("🧊", "HDR     ", "Linear pipeline + tonemapping");
	print_line_features("🧩", "Accel   ", "BVH (SAH)");
	print_geometry_lines();
	print_line_features("🪟", "Display ", "MiniLibX (X11)");
	print_line_features("🎥", "Camera  ", "6-DoF navigation");
	print_line_features("🎮", "Control ", "Orbit / Pan / Dolly / Look / Roll / Level");
	ft_putstr_fd("\033[1;34m", 1);
	ft_putstr_fd("  └─────────────────────────────────────────────────────────────┘\n", 1);
	ft_putstr_fd("\033[1;32m", 1);
	ft_putstr_fd("\n\n\n✨ Starting MiniRT...✨\n", 1);
	ft_putstr_fd("\033[0m", 1);
}


static void	print_banner(void)
{
	print_clear();
	print_cyan_header();
	print_features_box();
}


static t_logsec	step_section(t_logstep step)
{
	if (step == LOGSTEP_CHECK_ARGS)
		return (LOGSEC_BOOT);
	if (step >= LOGSTEP_PARSE_INIT_SCENE && step <= LOGSTEP_PARSE_FREE_PARSED)
		return (LOGSEC_PARSE);
	if (step >= LOGSTEP_ENGINE_INIT_CAM_VIEWS && step <= LOGSTEP_ENGINE_RENDER_INIT)
		return (LOGSEC_ENGINE);
	return (LOGSEC_DISPLAY);
}

static const char	*section_header(t_logsec section)
{
	if (section == LOGSEC_PARSE)
		return (LOG_HDR_PARSE);
	if (section == LOGSEC_ENGINE)
		return (LOG_HDR_ENGINE);
	return (LOG_HDR_DISPLAY);
}

static int	section_total(t_logsec section)
{
	if (section == LOGSEC_PARSE)
		return (LOG_PARSE_TOTAL);
	if (section == LOGSEC_ENGINE)
		return (LOG_ENGINE_TOTAL);
	return (LOG_DISPLAY_TOTAL);
}

static const char	*step_label(t_logstep step)
{
	if (step == LOGSTEP_PARSE_INIT_SCENE)
		return ("INIT_SCENE");
	if (step == LOGSTEP_PARSE_READ_FILE)
		return ("READ_FILE");
	if (step == LOGSTEP_PARSE_BUILD_CONVERT)
		return ("BUILD_CONVERT");
	if (step == LOGSTEP_PARSE_FREE_PARSED)
		return ("FREE_PARSED");
	if (step == LOGSTEP_ENGINE_INIT_CAM_VIEWS)
		return ("INIT_CAM_VIEWS");
	if (step == LOGSTEP_ENGINE_INIT_BVH)
		return ("INIT_BVH");
	if (step == LOGSTEP_ENGINE_RENDER_INIT)
		return ("RENDER_INIT");
	if (step == LOGSTEP_DISPLAY_INIT_CORE)
		return ("INIT_CORE");
	if (step == LOGSTEP_DISPLAY_INIT_FRAMES)
		return ("INIT_FRAMES");
	if (step == LOGSTEP_DISPLAY_INIT_LOOP)
		return ("INIT_LOOP");
	if (step == LOGSTEP_DISPLAY_LOAD_TEXTURES)
		return ("LOAD_TEXTURES");
	if (step == LOGSTEP_DISPLAY_INIT_CAM_CTRL)
		return ("INIT_CAM_CTRL");
	return ("INIT_SKY");
}

static void	print_token(int status)
{
	if (status == 0)
		ft_putstr_fd(LOG_GREEN "[" LOG_TOK_OK "]" LOG_RESET, 1);
	else
		ft_putstr_fd(LOG_RED "[" LOG_TOK_KO "]" LOG_RESET, 1);
}

static void	print_step_line(int idx, int total, const char *label, int status)
{
	ft_putstr_fd("\t[", 1);
	ft_putnbr_fd(idx, 1);
	ft_putstr_fd("/", 1);
	ft_putnbr_fd(total, 1);
	ft_putstr_fd("] ", 1);
	print_token(status);
	ft_putstr_fd(" ", 1);
	ft_putstr_fd(label, 1);
	ft_putstr_fd("\n", 1);
}

static void	print_check_args(int status)
{
	ft_putstr_fd("  ", 1);
	if (status == 0)
		ft_putstr_fd(LOG_GREEN "OK" LOG_RESET, 1);
	else
		ft_putstr_fd(LOG_RED "KO" LOG_RESET, 1);
	ft_putstr_fd(" check arguments\n", 1);
}

static void	handle_section_step(t_logsec section, int *idx, int *printed,
			t_logstep step, int status)
{
	if (*printed == 0)
	{
		ft_putstr_fd("\n", 1);
		ft_putstr_fd(section_header(section), 1);
		ft_putstr_fd("\n", 1);
		*printed = 1;
	}
	print_step_line((*idx) + 1, section_total(section), step_label(step), status);
	if (status == 0)
		(*idx)++;
}

void	log_step(t_logstep step, int status)
{
	static int	latch_error = 0;
	static int	idx_parse = 0;
	static int	idx_engine = 0;
	static int	idx_display = 0;
	static int	printed_parse = 0;
	static int	printed_engine = 0;
	static int	printed_display = 0;
	t_logsec	section;

	if (latch_error)
		return ;
	if (step == LOGSTEP_BOOT)
	{
		print_banner();
		return ;
	}
	section = step_section(step);
	if (section == LOGSEC_BOOT)
		print_check_args(status);
	else if (section == LOGSEC_PARSE)
		handle_section_step(section, &idx_parse, &printed_parse, step, status);
	else if (section == LOGSEC_ENGINE)
		handle_section_step(section, &idx_engine, &printed_engine, step, status);
	else
		handle_section_step(section, &idx_display, &printed_display, step, status);
	if (status != 0)
		latch_error = 1;
}
