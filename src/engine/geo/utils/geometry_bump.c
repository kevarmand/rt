/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   geometry_bump.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 19:23:19 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/19 17:42:29 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../geometry.h"

static void	surface_grad_to_base(const t_surface *surface,
				t_vec3f grad, t_vec3f *out)
{
	out->x = surface->uv_jacobian_t[0] * grad.x
		+ surface->uv_jacobian_t[1] * grad.y;
	out->y = surface->uv_jacobian_t[2] * grad.x
		+ surface->uv_jacobian_t[3] * grad.y;
	out->z = 0.0f;
}

void	bump_apply_normal(const t_bump_apply *bump, t_vec3f *normal)
{
	t_vec3f	grad;
	t_vec3f	base;

	grad = sample_texture(&bump->scene->bumpmaps[bump->surface->bump_normal_id],
			bump->u, bump->v);
	surface_grad_to_base(bump->surface, grad, &base);
	*normal = vec3f_add(*normal, vec3f_scale(bump->tangent,
				base.x * bump->surface->bump_scale));
	*normal = vec3f_add(*normal, vec3f_scale(bump->bitangent,
				base.y * bump->surface->bump_scale));
	*normal = vec3f_normalize(*normal);
}
