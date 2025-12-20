/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_config.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 21:59:50 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/19 21:54:37 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_CONFIG_H
# define RT_CONFIG_H

# define RT_NAME "RayonTraceur 2999.7"

# define TILE_SIZE 64
# define MAX_WORKER_THREADS 25
# define RGB_CHANNELS 3
# define SCROLL_PIXEL 120.0f
# ifndef EPSILON
#  define EPSILON 0x1.0c6f7ap-20f
# endif
# define TMIN_PRIM 1e-4f
# define TMIN_SHADOW 1e-3f
# define TMIN_SHADOW_BIAS 5e-5f

# define MAX_RECURSION_DEPTH 20
# define MIN_CONTRIBUTION 0.001f

//Tonemapping
# define TONESIZE 512

#endif
