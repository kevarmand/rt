/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_types.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: norivier <norivier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 02:17:10 by norivier          #+#    #+#             */
/*   Updated: 2025/05/12 18:58:56 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_TYPES_H
# define FT_TYPES_H 1
# include <stddef.h>
# include <stdint.h>
# include <endian.h>

typedef unsigned long int __attribute__((may_alias))	t_op;
typedef unsigned char									t_byte;
//# define OPSIZ	(sizeof(t_op))
enum
{
	OPSIZ = sizeof(t_op)
};

/* Threshold value for when to enter the unrolled loops. */
# define OP_T_THRES 16

#endif // FT_TYPES_H
