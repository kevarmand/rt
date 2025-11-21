/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_hit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 21:25:56 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/21 21:47:11 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"

int scene_hit(t_scene *scene, const t_ray *ray, t_hit *out_hit)
{
	int hit;
	
	//parcourir tout les primitives de la scene et tester lintersection 
	//on commence par les plans ( rapide a test et peuxx couper )
	//puis les autres primitives

	hit = inter_planes(scene, ray, out_hit);
	hit |= inter_primitives(scene, ray, out_hit);
	return (hit);
}