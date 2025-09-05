#ifndef TRACER_H
#define TRACER_H

#include "rt.h"
#include "type.h"

typedef struct s_ray2 {
	t_point   origin;
	t_vector  direction;     /* normalisée */
	int       depth;
	double    eta;   /* indice milieu courant (1.0 air) */
}	t_ray;

typedef struct s_hit {
	void     *obj;
	double    distance;
	t_point   position;
	t_vector  normal;
	int       inside;
}	t_hit;


int	ray_trace(t_data *data, const t_ray *ray, t_color *out);

#endif