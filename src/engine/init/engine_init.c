/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 13:04:56 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 07:33:14 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "engine.h"
#include "errors.h"
#include "bvh.h"
#include "libft.h"
#include "logs.h"

#define RENDER_MODE_NORMAL 0

static void	engine_set_defaults(t_engine *engine, const t_scene *scene)
{
	engine->width = scene->resolution_width;
	engine->height = scene->resolution_height;
	engine->active_cam = 0;
	engine->render_mode = RENDER_MODE_NORMAL;
	engine->samples_per_pixel = 1;
}

void	cleanup_bvh_buf(t_bvh_buf *buf)
{
	free(buf->pref);
	free(buf->pref_idx);
}

static int	init_bvh(t_scene *scene)
{
	t_primitive	*tmp;

	__attribute__((cleanup(cleanup_bvh_buf))) t_bvh_buf buf ;
	ft_memset(&buf, 0, sizeof(t_bvh_buf));
	scene->bvh_nodes = malloc(sizeof(t_bvhnode) * scene->primitive_count);
	if (scene->bvh_nodes == NULL)
		return (ERR_MALLOC);
	buf.pref = malloc(sizeof(t_primref) * scene->primitive_count);
	if (buf.pref == NULL)
		return (ERR_MALLOC);
	buf.pref_idx = malloc(sizeof(int) * scene->primitive_count);
	if (buf.pref_idx == NULL)
		return (ERR_MALLOC);
	prim_bound_init(&buf, scene->primitives, scene->primitive_count);
	build_bvh(scene->bvh_nodes, &buf, scene->primitive_count);
	scene->bvh_node_count = buf.nodecount;
	scene->bvh_root_id = 0;
	tmp = reorder_prims(scene->primitives, buf.pref_idx,
			scene->primitive_count);
	if (tmp == NULL)
		return (ERR_MALLOC);
	free(scene->primitives);
	scene->primitives = tmp;
	return (0);
}

int	engine_init(t_engine *engine, t_scene *scene)
{
	int	status;

	engine_set_defaults(engine, scene);
	init_cam_views(scene);
	log_step(LOGSTEP_ENGINE_INIT_CAM_VIEWS, 0);
	status = 0;
	if (scene->primitive_count)
		status = init_bvh(scene);
	if (status == SUCCESS)
		perr(ERR_MALLOC, PERR_BVH_INIT);
	log_step(LOGSTEP_ENGINE_INIT_BVH, 0);
	if (status != SUCCESS)
		return (status);
	status = render_init(&engine->render, engine->width, engine->height);
	log_step(LOGSTEP_ENGINE_RENDER_INIT, status);
	if (status != SUCCESS)
		return (status);
	return (SUCCESS);
}
