/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logs_steps1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: norivier <norivier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 07:31:23 by norivier          #+#    #+#             */
/*   Updated: 2025/12/21 07:33:46 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "logs.h"
#include "libft.h"

static void	print_features_box(void)
{
	ft_putstr_fd("\n\033[1;34m", 1);
	ft_putstr_fd("  ┌────────────────────────────────────────────────────"
		"──────────┐\n", 1);
	print_line_features("🧠", "Shading ", "Phong / Specular / Ambient");
	print_line_features("💡", "Lights  ", "Colored lights");
	print_line_features("🌑", "Shadows ", "Hard shadows");
	print_line_features("🪞", "Optics  ", "Reflection / Refraction");
	print_line_features("🧱", "Surfaces", "Texture / Checkerboard / "
		"Bump mapping");
	print_line_features("🧊", "HDR     ", "Linear pipeline + tonemapping");
	print_line_features("🧩", "Accel   ", "BVH (SAH)");
	print_geometry_lines();
	print_line_features("🪟", "Display ", "MiniLibX (X11)");
	print_line_features("🎥", "Camera  ", "6-DoF navigation");
	print_line_features("🎮", "Control ", "Orbit / Pan / Dolly / Look / "
		"Roll / Level");
	ft_putstr_fd("\033[1;34m", 1);
	ft_putstr_fd("  └───────────────────────────────────────────────────"
		"──────────┘\n", 1);
	ft_putstr_fd("\033[1;32m", 1);
	ft_putstr_fd("\n\n\n✨ Starting MiniRT...✨\n", 1);
	ft_putstr_fd("\033[0m", 1);
}

void	print_banner(void)
{
	print_clear();
	print_cyan_header();
	print_features_box();
}

t_logsec	step_section(t_logstep step)
{
	if (step == LOGSTEP_CHECK_ARGS)
		return (LOGSEC_BOOT);
	if (step >= LOGSTEP_PARSE_INIT_SCENE && step <= LOGSTEP_PARSE_FREE_PARSED)
		return (LOGSEC_PARSE);
	if (step >= LOGSTEP_ENGINE_INIT_CAM_VIEWS
		&& step <= LOGSTEP_ENGINE_RENDER_INIT)
		return (LOGSEC_ENGINE);
	return (LOGSEC_DISPLAY);
}

const char	*section_header(t_logsec section)
{
	if (section == LOGSEC_PARSE)
		return (LOG_HDR_PARSE);
	if (section == LOGSEC_ENGINE)
		return (LOG_HDR_ENGINE);
	return (LOG_HDR_DISPLAY);
}

int	section_total(t_logsec section)
{
	if (section == LOGSEC_PARSE)
		return (LOG_PARSE_TOTAL);
	if (section == LOGSEC_ENGINE)
		return (LOG_ENGINE_TOTAL);
	return (LOG_DISPLAY_TOTAL);
}
