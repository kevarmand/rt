/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_triangle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 16:46:49 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 08:44:14 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include "errors.h"
#include "../parsing_internal.h"
#include "vector.h"

// static int	vec3f_equal(t_vec3f a, t_vec3f b)
// {
// 	return (a.x == b.x && a.y == b.y && a.z == b.z);
// }

// static int	check_triangle(t_parsed_triangle *t)
// {
// 	t_vec3f a;
// 	t_vec3f b;
// 	t_vec3f c;
// 	t_vec3f u;
// 	t_vec3f v;
// 	t_vec3f n;

// 	a = (t_vec3f){t->vertex1[0], t->vertex1[1], t->vertex1[2]};
// 	b = (t_vec3f){t->vertex2[0], t->vertex2[1], t->vertex2[2]};
// 	c = (t_vec3f){t->vertex3[0], t->vertex3[1], t->vertex3[2]};
// 	if (vec3f_equal(a, b) || vec3f_equal(a, c) || vec3f_equal(b, c))
// 		return (ERR_PARS);
// 	u = vec3f_sub(b, a);
// 	v = vec3f_sub(c, a);
// 	n = vec3f_cross(u, v);
// 	if (vec3f_length(n) < 0.000001f)
// 		return (ERR_PARS);
// 	return (0);
// }

int	pars_triangle(t_pars_state *st, t_scene_parsed *scene)
{
	t_parsed_element	parsed_element;
	t_tok				token;
	int					status;

	init_parsed_element(&parsed_element, scene);
	parsed_element.type = ELEM_TRIANGLE;
	if (!pars_next_tok(st, &token) || scan_point(token,
			parsed_element.u_data.triangle.vertex1))
		return (ERR_PARS);
	if (!pars_next_tok(st, &token) || scan_point(token,
			parsed_element.u_data.triangle.vertex2))
		return (ERR_PARS);
	if (!pars_next_tok(st, &token) || scan_point(token,
			parsed_element.u_data.triangle.vertex3))
		return (ERR_PARS);
	if (!pars_next_tok(st, &token) || scan_color(token,
			parsed_element.rgb))
		return (ERR_PARS);
	status = pars_options(st, &parsed_element.options, &scene->textures);
	if (status != 0)
		return (ERR_PARS);
	return (pars_register_element(scene, &parsed_element, ELEM_ROLE_NORMAL));
}
