/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmand <karmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/10 14:01:53 by karmand           #+#    #+#             */
/*   Updated: 2020/01/11 14:25:39 by karmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static int		m_err(char *txt)
{
	int i;

	i = 0;
	while (txt[i])
		i++;
	write(2, txt, i);
	return (0);
}

int 			error(int code)
{
	m_err("Error\n");
	if (code == ERROR_OPEN)
		return (m_err("Cannot read the file\n"));
	if (code == ERROR_ARGUMENT)
		return (m_err("Bad argument\n"));
	if (code == ERROR_MALLOC)
		return (m_err("Malloc-Erreur d'allocation dynamique"));
	if (code == ERROR_PARS_COLOR)
		return (m_err("Parsing COLOR:la range doit etre comprise entre [0-255]"));
	if (code == ERROR_PARS_POINT)
		return (m_err("Parsing - Le format du point de correspond pas"));
	return (0);
}
