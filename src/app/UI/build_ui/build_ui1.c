/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ui1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 09:00:34 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 09:00:52 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "build_ui.h"

char	*ui_ssaa_txt(int idx)
{
	if (idx == UI_SSAA_IDX_MIN)
		return ("1");
	if (idx == UI_SSAA_IDX_MIN + 1)
		return ("4");
	if (idx == UI_SSAA_IDX_MIN + 2)
		return ("9");
	if (idx == UI_SSAA_IDX_MIN + 3)
		return ("16");
	return ("25");
}

char	*ui_onoff_txt(int enabled)
{
	if (enabled)
		return (UI_TXT_ON);
	return (UI_TXT_OFF);
}

int	ui_is_mode_selected(t_ui *ui, int mode_idx)
{
	if (ui->mode_selected == mode_idx)
		return (1);
	return (0);
}

int	ui_is_render_selected(t_ui *ui, int render_mode)
{
	if (ui->render_mode_selected == render_mode)
		return (1);
	return (0);
}

void	ui_push_button(t_ui *ui, t_button *btn)
{
	ui->buttons[ui->button_count] = *btn;
	ui->button_count++;
}