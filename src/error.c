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

	if ( n > 9)
		n_err(n / 10);
	i = (n % 10) + '0';
	write(2, &i, 1);
	return (SUCCESS);
}

int 			error(int code)
{
	m_err("Erreur - ");
	if (code == ERR_MALLOC)
		return (m_err("Malloc : Erreur d'allocation dynamique\n"));
	

	if (code == ERROR_OPEN)
		return (m_err("Cannot read the file\n"));
	if (code == ERROR_ARGUMENT)
		return (m_err("Bad argument\n"));
	if (code == ERROR_PARS_COLOR)
		return (m_err("Parsing COLOR:la range doit etre comprise dans [0-255]"));
	if (code == ERROR_PARS_POINT)
		return (m_err("Parsing - Le format du point de correspond pas"));
	if (code == ERROR_INT)
		return (m_err("Parsing : must be an integer"));
	return (m_err("non specifié"));
}

int				m_err2(char **arr)
{
	int		n;

	n = 0;
	while (arr[n])
		m_err(arr[n]);
	return (-1);
}

int				err2(int code, char *s, int n)
{
	if (code == ERR_MALLOC)
		return (-1);
	m_err("Erreur à la ligne ");
	n_err(n);
	m_err(" : ");
	m_err(s);
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
	if (code == ERR_PL)
		return (m_err("pl\t\tx,y,z\t\tX,Y,Z\t\tr,g,b\n"));
	if (code == ERR_TR)
		return (m_err("tr\t\tx1,y1,z1\t\tx2,y2,z2\t\tx3,y3,z3\t\tr,g,b\n"));
}



int				err1(int code, char *s)
{
	m_err("Erreur - ");
	if (code = ERR_COLOR)
	{
		m_err("Champs (couleur) : format attendu [x,x,x], x ");
		m_err("entier compris entre 0 et 255   ==>   ");
		m_err(s);
		m_err("\n");
	}
}

