/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_config.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 17:07:05 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/10 18:51:34 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_CONFIG_H
# define RT_CONFIG_H

# define RT_NAME "RayonTraceur 2999.7"

# define TILE_SIZE 64
# define MAX_WORKER_THREADS 25
# define RGB_CHANNELS 3
# ifndef EPSILON
#  define EPSILON 0x1.0c6f7ap-20f
# endif

# define MAX_RECURSION_DEPTH 20
# define MIN_CONTRIBUTION 0.01f

#endif
