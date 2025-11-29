/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shade_hit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 21:21:27 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/29 13:01:48 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include "scene.h"

int	shade_hit(const t_scene *scene, const t_hit *hit, t_vec3f *color_out)
{
	t_vec3f	color;

	//on remet la couleur a 0
	color = (t_vec3f){0.0f, 0.0f, 0.0f};
	//On ajouter la part ambiante
	shade_ambient(scene, hit, &color); //Normalement ok ( mais possible davoirbesoin de , scene)
	//On ajoute la part diffuse et spéculaire pour chaque lumière
	shade_direct_lights(scene, hit, &color);
	
	// shade_Beer_Lambert(scene, hit, &color); <= not implemented yet

	// shade_reflection(scene, hit, &color);<= not implemented yet

	// shade_refraction(scene, hit, &color);<= not implemented yet
	
	//ect

	*color_out = color;
	return (0);
}