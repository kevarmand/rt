/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inter_torus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 14:42:55 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/22 17:33:17 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include <math.h>
#include "scene.h"

int	inter_torus(const t_torus *primitive,
			const t_ray *ray, float *out_distance)
{
	//Implémentation de l'intersection rayon-tore
	//Pour l'instant, on retourne toujours 0 (pas d'intersection)
	(void)primitive;
	(void)ray;
	(void)out_distance;
	return (0);
}