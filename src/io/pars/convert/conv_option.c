/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conv_option.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 10:40:46 by kearmand          #+#    #+#             */
/*   Updated: 2025/10/06 10:57:29 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include "io.h"
#include "error_codes.h"
#include <stdlib.h>

//Docn el but cest de gere une option dans une hashmap.
// on creer un vector d option et une hashmap ( en creeant des clefs unique, afin de parser rapidement )
// la ou t_option est une structure qui contiens toutes les options, dans l objet final on va avoir un index vers un tableau de t_materialet de t_surface
// ou chaque element de ce tableau est unique ( pas de duplication d option )

int	conv_option(t_scene *scene, t_element_options *options)
{

}