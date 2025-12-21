/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logs_steps.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 23:37:57 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 07:36:32 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "logs.h"
#include "libft.h"

void	print_clear(void)
{
	ft_putstr_fd("\033[2J\033[H", 1);
}

void	print_cyan_header(void)
{
	ft_putstr_fd("\033[1;36m\n", 1);
	ft_putstr_fd("╔═══════════════════════════════════════════════════════════"
		"═══════════════════╗\n", 1);
	ft_putstr_fd("║                                                           "
		"                   ║\n", 1);
	ft_putstr_fd("║                ███╗   ███╗██╗███╗   ██╗██╗██████╗ ████████"
		"╗                  ║\n", 1);
	ft_putstr_fd("║                ████╗ ████║██║████╗  ██║██║██╔══██╗╚══██╔══"
		"╝                  ║\n", 1);
	ft_putstr_fd("║                ██╔████╔██║██║██╔██╗ ██║██║██████╔╝   ██║  "
		"                   ║\n", 1);
	ft_putstr_fd("║                ██║╚██╔╝██║██║██║╚██╗██║██║██╔══██╗   ██║  "
		"                   ║\n", 1);
	ft_putstr_fd("║                ██║ ╚═╝ ██║██║██║ ╚████║██║██║  ██║   ██║  "
		"                   ║\n", 1);
	ft_putstr_fd("║                ╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝╚═╝  ╚═╝   ╚═╝  "
		"                   ║\n", 1);
	ft_putstr_fd("║                                                           "
		"                   ║\n", 1);
	ft_putstr_fd("║                          by kearmand - norivier           "
		"                   ║\n", 1);
	ft_putstr_fd("║                                                           "
		"                   ║\n", 1);
	ft_putstr_fd("╚═══════════════════════════════════════════════════════════"
		"═══════════════════╝\n", 1);
}

void	print_line_features(const char *logo,
		const char *label, const char *value)
{
	ft_putstr_fd("  \033[1;34m│\033[1;33m  ", 1);
	ft_putstr_fd(logo, 1);
	ft_putstr_fd("  ", 1);
	ft_putstr_fd(label, 1);
	ft_putstr_fd(" : \033[1;37m", 1);
	ft_putstr_fd(value, 1);
	ft_putstr_fd("\033[1;33m\n", 1);
}

void	print_geometry_lines(void)
{
	ft_putstr_fd("  \033[1;34m│\033[1;33m  🧬  Geometry :  \033[1;37m🟠 "
		"Sphere     🟦 Plane\033[1;33m\n", 1);
	ft_putstr_fd("  \033[1;34m│\033[1;33m                  \033[1;37m🍩 "
		"Torus      🛢️ Cylinder\033[1;33m\n", 1);
	ft_putstr_fd("  \033[1;34m│\033[1;33m                        \033[1;37m🔺 "
		"Triangle   (Möller–Trumbore)\033[1;33m\n", 1);
}

typedef struct s_log_step_ctx
{
	int	latch_error;
	int	idx_parse;
	int	idx_engine;
	int	idx_display;
	int	printed_parse;
	int	printed_engine;
	int	printed_display;
}	t_log_step_ctx;

void	log_step(t_logstep step, int status)
{
	static t_log_step_ctx	c;
	t_logsec				section;

	if (c.latch_error)
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
		handle_section_step(section, (int []){c.idx_parse, c.printed_parse},
			step, status);
	else if (section == LOGSEC_ENGINE)
		handle_section_step(section, (int []){c.idx_engine, c.printed_engine},
			step, status);
	else
		handle_section_step(section, (int []){c.idx_display, c.printed_display},
			step, status);
	if (status != 0)
		c.latch_error = 1;
}
