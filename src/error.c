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
	int		i;

	i = 0;
	while (txt[i])
		i++;
	write(2, txt, i);
	return (-1);
}

int				n_err(int n)
{
	char	i;

	if (n > 9)
		n_err(n / 10);
	i = (n % 10) + '0';
	write(2, &i, 1);
	return (SUCCESS);
}

int				error(int code)
{
	if (code == ERR_MALLOC)
		return (m_err("Erreur Malloc : Erreur d'allocation dynamique\n"));
	if (code == ERR_NORM)
		return (m_err("Erreur de norm nulle\n"));
	if (code == ERR_ARG)
		return (m_err("Erreur - Mauvais arguments\n"));
	if (code == ERR_MIN)
		return (m_err("Erreur - Le fichier de scene n'est pas complet\n"));
	if (code == ERR_MLX)
		return (m_err("MLX\n"));
	if (code == ERR_BMP)
		return (m_err("Impossible de creer l'image (.bmp)\n"));
	if (code == ERR_PL)
		return (m_err("pl\t\tx,y,z\t\tX,Y,Z\t\tr,g,b\n"));
	if (code == ERR_TR)
		return (m_err("tr\t\tx1,y1,z1\t\tx2,y2,z2\t\tx3,y3,z3\t\tr,g,b\n"));
	if (code == ERR_CY)
		return (m_err("cy\t\tx,y,z\t\tX,Y,Z\t\tr,g,b\t\tD\t\tH\n"));
	if (code == ERR_SQ)
		return (m_err("sq\t\tx,y,z\t\tX,Y,Z\t\tH\t\t\tr,g,b\n"));
	return (0);
}

int				err2(int code, char *s, int n)
{
	if (code == ERR_MALLOC)
		return (-1);
	m_err("Erreur à la ligne ");
	n_err(n);
	m_err(" : ");
	m_err(s);
	if (code == ERR_A_REDEF)
		return (m_err("\nRedefinition de A\n"));
	if (code == ERR_R_REDEF)
		return (m_err("\nRedefinition de R\n"));
	if (code == ERR_PARS_TYPE)
		return (m_err("\nformat inconnu\n"));
	m_err("\nLe format attendu est : ");
	if (code == ERR_A)
		return (m_err("A\t\t{0;1}\t\tr,g,b\n"));
	if (code == ERR_R)
		return (m_err("R\t\t[0-5000]\t\t[0-2000]\n"));
	if (code == ERR_C)
		return (m_err("c\t\tx,y,z\t\tX,Y,Z\t\t[0-180]\n"));
	if (code == ERR_L)
		return (m_err("l\t\tx,y,z\t\t{0,1}\t\tr,g,b\n"));
	if (code == ERR_SP)
		return (m_err("sp\t\tx,y,z\t\t{0,+}\t\tr,g,b\n"));
	return (error(code));
}

int				err_op(int code, char *s)
{
	if (code == ERR_OPEN)
	{
		m_err("Erreur dans l'ouverture du fichier : ");
		m_err(s);
		return (m_err("\n"));
	}
	if (code == ERR_EXT)
	{
		m_err("Erreur dans l'extension du fichier(.rt) : ");
		m_err(s);
		return (m_err("\n"));
	}
	return (0);
}
