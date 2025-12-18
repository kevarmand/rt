/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: norivier <norivier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 21:26:09 by norivier          #+#    #+#             */
/*   Updated: 2025/10/02 12:03:00 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H

# if defined(_MSC_VER)
#  define FORCEINLINE __forceinline
# elif defined(__GNUC__) || defined(__clang__)
#  define FORCEINLINE __attribute__((always_inline))
# endif

#endif // !TYPES_H
