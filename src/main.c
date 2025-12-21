/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 16:20:19 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 07:40:00 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "new_rt.h"
#include "errors.h"
#include "libft.h"
#include "logs.h"

void	destroy_scene(t_scene *scene);

int	main(int argc, char **argv)
{
	t_data	data;
	int		status;

	log_step(LOGSTEP_BOOT, 0);
	ft_memset(&data, 0, sizeof(t_data));
	status = check_args(argc, argv);
	log_step(LOGSTEP_CHECK_ARGS, status);
	if (status == SUCCESS)
		status = load_scene(argv[1], &data.scene);
	if (status == SUCCESS)
		status = run_app(&data);
	destroy_scene(&data.scene);
	return (status);
}
