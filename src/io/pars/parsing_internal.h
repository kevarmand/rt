/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_internal.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 13:18:27 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/20 18:27:22 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef PARSING_INTERNAL_H
# define PARSING_INTERNAL_H

typedef struct s_pars_state {
	int			line_no;
	int			depth;
	const char	*line;
	int			pos;
}	t_pars_state;

typedef struct s_tok {
	const char	*start;
	int			len;
}	t_tok;

/* ************************************************************************** */
/*                             PARSING - LIFETIME                             */
/* ************************************************************************** */

/**
 * @brief Initialize a t_scene_parsed structure with default values.
 * @param scene Pointer to the t_scene_parsed structure to initialize. (no_null)
 */
void	init_parsed_scene(t_scene_parsed *parsed);

/***
 * @brief Free all dynamically allocated memory in a t_scene_parsed structure.
 * @param parsed Pointer to the t_scene_parsed structure to free. (no_null)
 */
void	free_parsed_scene(t_scene_parsed *parsed);

/***
 * @brief Initialize a t_parsed_element structure with default values.
 * @param elem Pointer to the t_parsed_element structure to initialize. (no_null)
 */
void	init_parsed_element(t_parsed_element *elem);

/***
 * @brief Free any dynamically allocated memory within a t_parsed_element.
 * @param elem Pointer to the t_parsed_element to free. (no_null)
 */
void	free_parsed_element(void *obj);


#endif