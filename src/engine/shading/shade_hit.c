/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shade_hit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 21:21:27 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/21 21:21:36 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include "scene.h"

int	shade_hit(t_scene *scene, t_hit *hit, t_vec3f *color_out)
{
	//Pour l'instant on met une couleur fixe pour tester
	(void)scene;
	(void)hit;
	(*color_out)[0] = 1.0f; //rouge
	(*color_out)[1] = 0.0f;
	(*color_out)[2] = 0.0f;
	return (0);
}