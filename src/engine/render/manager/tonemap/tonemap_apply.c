/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tonemap_apply.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 17:15:24 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/17 17:15:25 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tonemap_apply.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 15:41:36 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/17 17:05:34 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tonemap_internal.h"
#include "color.h"
#include <math.h>

float	tm_exposure_from_lum(float lum_ref, float key)
{
	if (lum_ref <= TM_EPS)
		return (1.0f);
	return (key / lum_ref);
}

float	tm_filmic_exp(float lum, float exposure)
{
	float	x;

	x = lum * exposure;
	if (x <= 0.0f)
		return (0.0f);
	return (1.0f - expf(-x));
}

