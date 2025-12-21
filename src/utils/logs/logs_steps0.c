/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logs_steps0.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: norivier <norivier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 07:28:02 by norivier          #+#    #+#             */
/*   Updated: 2025/12/21 07:30:51 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "logs.h"
#include "libft.h"

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

void	print_check_args(int status)
{
	ft_putstr_fd("  ", 1);
	if (status == 0)
		ft_putstr_fd(LOG_GREEN "OK" LOG_RESET, 1);
	else
		ft_putstr_fd(LOG_RED "KO" LOG_RESET, 1);
	ft_putstr_fd(" check arguments\n", 1);
}

void	handle_section_step(t_logsec section, int *idxprinted[],
			t_logstep step, int status)
{
	if (idxprinted[1][0] == 0)
	{
		ft_putstr_fd("\n", 1);
		ft_putstr_fd(section_header(section), 1);
		ft_putstr_fd("\n", 1);
		idxprinted[1][0] = 1;
	}
	print_step_line((idxprinted[0][0]) + 1, section_total(section),
		step_label(step), status);
	if (status == 0)
		(idxprinted[0][0])++;
}
