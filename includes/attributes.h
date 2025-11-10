/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   attributes.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 21:26:09 by norivier          #+#    #+#             */
/*   Updated: 2025/11/10 14:19:19 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ATTRIBUTES_H

# if defined(_MSC_VER)
#  define FORCEINLINE __forceinline
# elif defined(__GNUC__) || defined(__clang__)
#  define FORCEINLINE __attribute__((always_inline))
# endif

#endif // !ATTRIBUTES_H
