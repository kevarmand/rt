/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conv_option.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 10:40:46 by kearmand          #+#    #+#             */
/*   Updated: 2025/10/16 13:53:23 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include "io.h"
#include "error_codes.h"
#include <stdlib.h>
#include "libft.h"
#include "scene.h"


// on creer un vector d option et une hashmap ( en creeant des clefs unique, afin de parser rapidement )
// la ou t_option est une structure qui contiens toutes les options, dans l objet final on va avoir un index vers un tableau de t_materialet de t_surface
// ou chaque element de ce tableau est unique ( pas de duplication d option )

int	conv_option(t_scene *scene, t_element_options *options)
{
	//0. si il y  a des texture ( abedo ou bumpmap ) on doit les ajouter et recuperer leur index)
	
	//1. generer une clef unique pour cette option

	//2. chercher dans la hashmap si cette clef existe deja
	
	//3. si oui, retourner l index
	//4. si non, creer un nouveau material, l ajouter au vector et a la hashmap
	
	//5. faire les surfaces aussi

	return (0);
}