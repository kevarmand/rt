/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parse.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmand <karmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 17:25:55 by karmand           #+#    #+#             */
/*   Updated: 2020/02/29 22:20:00 by karmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	test_print_color(t_color *c)
{
	printf("(%i,%i,%i)", c->r, c->g, c->b);
}

void	test_print_point(t_point *p)
{
	printf("(x = %.2f, y = %.2f, z = %.2f)", p->x, p->y, p->z);
}	

void	test_print_enum(void *obj, t_type t)
{
	if (t == PLANE)
	{	
		t_pl *pl = (t_pl *)obj;
		printf("PLANE\n");
		printf(" -Origine ");
		test_print_point(&(pl->point));
		printf("\n -Vector ");
		test_print_point((t_point *)&(pl->norm));
		printf("\n -Color ");
		test_print_color(&(pl->color));
		printf("\n\n");

	}
	if (t == CILYNDER)
		printf("CILYNDER");
	if (t == TRIANGLE)
	{
		t_tr *tr = (t_tr *)obj;
		printf("TRIANGLE");
		printf("\n -p1 ");
		test_print_point(&(tr->p1));
		printf("\n -p2 ");
		test_print_point(&(tr->p2));
		printf("\n -p3 ");
		test_print_point(&(tr->p3));
		printf("\n -Color ");
		test_print_color(&(tr->color));
		printf("\n\n");
	}
	if (t == SQUARE)
		printf("SQUARE");
	if (t == CAM)
	{
		printf("CAM :\n");
		printf(" -Origine ");
		test_print_point(&(((t_cam *)obj)->point));
		printf("\n -Dir ");
		test_print_point((t_point *)&(((t_cam *)obj)->dir));
		printf("\n -FoV ");
		printf("%f", ((t_cam *)obj)->fov);
		printf("°\n\n");
	}
	if (t == LIGHT)
	{
		printf("LIGHT :\n");
		printf(" -Origine ");
		test_print_point(&(((t_light *)obj)->point));
		printf("\n -Brightness : %.2f\n", ((t_light *)obj)->brightness);
		printf(" -Color ");
		test_print_color(&((t_light *)obj)->color);
		printf("\n\n");
	}
	if (t == SPHERE)
	{
		t_sp *sp = (t_sp *)obj;
		printf("SPHERE :\n");
		printf(" -Centre ");
		test_print_point(&(sp->centre));
		printf("\n -Rayon = %.2f\n", sp->rayon);
		printf(" -Color ");
		test_print_color(&(sp->color));
		printf("\n\n");
	}
}

void	test_afficher_obj(t_list *l)
{
	while (l != 0)
	{
		test_print_enum(l->obj, l->type);
		l = l->next;
	}
}

void	test_data_print(t_data *data)
{
	printf("\n\n\n\n\n\n\nAffichage de data\n\n");
	printf("Resolution : %i * %i\n", data->para.res_width, data->para.res_height);
	printf("Lumiere Ambiante : %f     ", data->para.brightness);
	test_print_color(&(data->para.color));
	printf("\n\n");
	test_afficher_obj(data->lcam);
	test_afficher_obj(data->llight);
	test_afficher_obj(data->lobj);
}

void	test_print_dist(t_ldist *p)
{
	printf("taille de n = %i\n", p->n);
	for(int i = 0; i < p->n; i++)
	{
		printf("n %i  : ", i);
		test_print_enum(p->plot[i].p, p->plot[i].type);
	}
}
