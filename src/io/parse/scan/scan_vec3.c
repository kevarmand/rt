/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan_vec3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 16:48:14 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/16 16:48:15 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "type.h"
#include "io.h"
#include "errors.h"
#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include "../parsing_internal.h"

static const char	g_col_orange[] = "\033[0;33m";
static const char	g_col_reset[] = "\033[0m";
static const char	g_warn_prefix[] = "Warning: ";
static const char	g_msg_range[] = "Vector component out of range [-1,1].\n";
static const char	g_msg_norm[] = "Normalizing non-unit vector.\n";
static const char	g_msg_zerolen[] = "Error: Zero-length vector cannot\
	be normalized.\n";

static void	put_warn_prefix(void)
{
	write(2, g_col_orange, sizeof(g_col_orange) - 1);
	write(2, g_warn_prefix, sizeof(g_warn_prefix) - 1);
	write(2, g_col_reset, sizeof(g_col_reset) - 1);
}

static void	check_range(const float vec[3])
{
	int	component_index;
	int	has_out;

	component_index = 0;
	has_out = 0;
	while (component_index < 3)
	{
		if (vec[component_index] < -1.0f || vec[component_index] > 1.0f)
			has_out = 1;
		component_index++;
	}
	if (has_out)
	{
		put_warn_prefix();
		write(2, g_msg_range, sizeof(g_msg_range) - 1);
	}
}

static int	normalise_vec(float vec[3])
{
	float	len2;
	float	inv_len;

	len2 = vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2];
	if (len2 < __FLT_EPSILON__)
	{
		write(2, g_msg_zerolen, sizeof(g_msg_zerolen) - 1);
		return (ERR_PARSE_FLOAT);
	}
	if (fabsf(len2 - 1.0f) >= __FLT_EPSILON__)
	{
		put_warn_prefix();
		write(2, g_msg_norm, sizeof(g_msg_norm) - 1);
		inv_len = 1.0f / sqrtf(len2);
		vec[0] *= inv_len;
		vec[1] *= inv_len;
		vec[2] *= inv_len;
	}
	return (SUCCESS);
}

int	scan_vec3(t_tok tok, float vec[3])
{
	float	tmp[3];

	if (scan_point(tok, tmp))
		return (ERR_PARSE_FLOAT);
	check_range(tmp);
	if (normalise_vec(tmp))
		return (ERR_PARSE_FLOAT);
	vec[0] = tmp[0];
	vec[1] = tmp[1];
	vec[2] = tmp[2];
	return (SUCCESS);
}
