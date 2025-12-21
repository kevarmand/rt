/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_ui.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 11:20:00 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 09:10:44 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "draw_ui.h"


void	draw_ui(t_ui *ui)
{
	int	index;

	ui_draw_base(ui);
	index = 0;
	while (index < ui->button_count)
	{
		ui_draw_one_button(ui, &ui->buttons[index]);
		index++;
	}
}
