<<<<<<< HEAD:src/engine/math/rt_math.h

=======
>>>>>>> readme:src/vector_math/rt_math.h
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_math.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: norivier <norivier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
<<<<<<< HEAD:src/engine/math/rt_math.h
<<<<<<<< HEAD:src/engine/math/rt_math.h
/*   Created: 2025/10/01 21:29:13 by norivier          #+#    #+#             */
/*   Updated: 2025/10/02 10:24:57 by norivier         ###   ########.fr       */
========
/*   Created: 2025/09/11 16:47:11 by norivier          #+#    #+#             */
/*   Updated: 2025/10/01 21:33:11 by norivier         ###   ########.fr       */
>>>>>>>> readme:src/vector_math/vector.h
=======
/*   Created: 2025/10/01 21:29:13 by norivier          #+#    #+#             */
/*   Updated: 2025/10/02 10:24:57 by norivier         ###   ########.fr       */
>>>>>>> readme:src/vector_math/rt_math.h
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_MATH_H
# define RT_MATH_H 1

<<<<<<< HEAD:src/engine/math/rt_math.h
<<<<<<<< HEAD:src/engine/math/rt_math.h
=======
>>>>>>> readme:src/vector_math/rt_math.h
#define EPSILON 0x1.0c6f7ap-20f
#define RCP_2 0x1p-1f
#define RCP_3 0x1.555556p-2f
#define RCP_4 0x1p-2f
#define RCP_8 0x1p-3f
#define RCP_9 0x1.c71c72p-4f
#define RCP_16 0x1p-4f
#define RCP_256 0x1p-8f
#define THREE_OVER_8 0x1.8p-2f
#define THREE_OVER_256 0x1.8p-2f

float	ft_rsqrtf(float x);
float	ft_rcpf(float x);

#endif // !RT_MATH_H
<<<<<<< HEAD:src/engine/math/rt_math.h
========
# if defined(__clang__)
typedef float t_vec4f __attribute__((ext_vector_type(4)));
typedef float t_mat3x3f __attribute__((matrix_type(3, 3)));
typedef float t_mat3x4f __attribute__((matrix_type(3, 4)));
# elif defined (__GNUC__)
typedef float t_vec4f __attribute__((vector_size(sizeof(float) * 4)));
# endif

typedef t_vec4f t_vec3f;

t_vec3f	vec3f_min(t_vec3f a, t_vec3f b);
t_vec3f	vec3f_max(t_vec3f a, t_vec3f b);
t_vec3f	vec3f_cross(t_vec3f a, t_vec3f b);
float	vec3f_dot(t_vec3f a, t_vec3f b);
t_vec3f	vec3f_scale(t_vec3f a, float s);
t_vec3f	vec3f_add(t_vec3f a, t_vec3f b);
t_vec3f	vec3f_sub(t_vec3f a, t_vec3f b);
t_vec3f	vec3f_normalize(t_vec3f a);

#endif // !VECTOR_H
>>>>>>>> readme:src/vector_math/vector.h
=======
>>>>>>> readme:src/vector_math/rt_math.h
