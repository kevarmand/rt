/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_hook.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmand <karmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 16:16:15 by karmand           #+#    #+#             */
/*   Updated: 2020/03/05 17:09:10 by karmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		key_1(int keycode, t_data *data, int err)
{
	if (keycode == KEY_ESCAPE)
		return (ft_exit(data));
	if (!err && keycode == KEY_TAB)
		err = 1 + select_next_cam(data);
	if (!err && keycode == KEY_CAPS)
		err = select_next_obj(data);
	if (!err && keycode == KEY_MAJ)
		err = select_next_light(data);
	if (!err && keycode == KEY_A)
		err = 1 + translate_switch(AXE_XM, data);
	if (!err && keycode == KEY_Z)
		err = 1 + translate_switch(AXE_XP, data);
	if (!err && keycode == KEY_Q)
		err = 1 + translate_switch(AXE_YM, data);
	if (!err && keycode == KEY_S)
		err = 1 + translate_switch(AXE_YP, data);
	if (!err && keycode == KEY_W)
		err = 1 + translate_switch(AXE_ZM, data);
	if (!err && keycode == KEY_X)
		err = 1 + translate_switch(AXE_ZP, data);
	if (!err && keycode == KEY_T)
		err = 1 + translate_light(((data->select).light)->obj, AXE_XM);
	if (!err && keycode == KEY_Y)
		err = 1 + translate_light(((data->select).light)->obj, AXE_XP);
	return (err);
}

int		key_2(int keycode, t_data *data, int err)
{
	if (!err && keycode == KEY_G)
		err = 1 + translate_light(((data->select).light)->obj, AXE_YM);
	if (!err && keycode == KEY_H)
		err = 1 + translate_light(((data->select).light)->obj, AXE_YP);
	if (!err && keycode == KEY_B)
		err = 1 + translate_light(((data->select).light)->obj, AXE_ZM);
	if (!err && keycode == KEY_N)
		err = 1 + translate_light(((data->select).light)->obj, AXE_ZP);
	if (!err && keycode == KEY_U)
		err = 1 + translate_cam(((data->select).cam)->obj, AXE_XM);
	if (!err && keycode == KEY_I)
		err = 1 + translate_cam(((data->select).cam)->obj, AXE_XP);
	if (!err && keycode == KEY_J)
		err = 1 + translate_cam(((data->select).cam)->obj, AXE_YM);
	if (!err && keycode == KEY_K)
		err = 1 + translate_cam(((data->select).cam)->obj, AXE_YP);
	if (!err && keycode == KEY_VIR)
		err = 1 + translate_cam(((data->select).cam)->obj, AXE_ZM);
	if (!err && keycode == KEY_POI)
		err = 1 + translate_cam(((data->select).cam)->obj, AXE_ZP);
	return (err);
}

int		key_3(int keycode, t_data *data, int err)
{
	if (!err && keycode == KEY_E)
		err = 1 + rotate_switch(AXE_XM, data);
	if (!err && keycode == KEY_R)
		err = 1 + rotate_switch(AXE_XP, data);
	if (!err && keycode == KEY_D)
		err = 1 + rotate_switch(AXE_YM, data);
	if (!err && keycode == KEY_F)
		err = 1 + rotate_switch(AXE_YP, data);
	if (!err && keycode == KEY_C)
		err = 1 + rotate_switch(AXE_ZM, data);
	if (!err && keycode == KEY_V)
		err = 1 + rotate_switch(AXE_ZP, data);
	if (!err && keycode == KEY_O)
		err = 2 + rotate_cam(((data->select).cam)->obj, AXE_XM);
	if (!err && keycode == KEY_P)
		err = 2 + rotate_cam(((data->select).cam)->obj, AXE_XP);
	if (!err && keycode == KEY_L)
		err = 2 + rotate_cam(((data->select).cam)->obj, AXE_YM);
	if (!err && keycode == KEY_M)
		err = 2 + rotate_cam(((data->select).cam)->obj, AXE_YP);
	if (!err && keycode == KEY_SLASH)
		err = 2 + rotate_cam(((data->select).cam)->obj, AXE_ZM);
	if (!err && keycode == KEY_EXC)
		err = 2 + rotate_cam(((data->select).cam)->obj, AXE_ZP);
	return (err);
}

int		key_4(int keycode, t_data *data, int err)
{
	if (!err && keycode == KEY_PLUS)
		err = 1 + resize_switch1(PLUS, data);
	if (!err && keycode == KEY_MOINS)
		err = 1 + resize_switch1(MOINS, data);
	if (!err && keycode == KEY_FOIS)
		err = 1 + resize_switch2(PLUS, data);
	if (!err && keycode == KEY_DIV)
		err = 1 + resize_switch2(MOINS, data);
	return (err);
}

int		key_hook(int keycode, t_data *data)
{
	int		err;

	err = 0;
	err = key_1(keycode, data, err);
	if (!err)
		err = key_2(keycode, data, err);
	if (!err)
		err = key_3(keycode, data, err);
	if (!err)
		err = key_4(keycode, data, err);
	if (err == 2)
		init_cam(((data->select).cam)->obj, data);
	if (err)
		render(data);
	return (SUCCESS);
}
