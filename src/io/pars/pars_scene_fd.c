#include "error_codes.h"
#include "type.h"
#include "libft.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


int	pars_scene_fd(int fd, t_data *data)
{
	int		err;

	if (fd < 0)
		return (ERR_OPEN);
	data_init(data);
	if ((err = parsing(fd, data)))
	{
		ft_data_exit(data);
		return (err);
	}
	if ((data->test)[1] == 0)
		err = ERR_NO_R;
	else if ((data->test)[2] == 0)
		err = ERR_NO_A;
	else if (data->lcam == 0)
		err = ERR_NO_C;
	else if (data->lobj == 0)
		err = ERR_EMPTY_OBJ;
	if (err)
	{
		ft_data_exit(data);
		return (err);
	}
	return (SUCCESS);
}

// On defini un structure t_scene provisoire : list chainee

//RAPELE DE LA STRUCT DE T_para
// typedef struct		s_para
// {
// 	int				res_width;
// 	int				res_height;
// 	double			brightness;
// 	t_color			color;
// }					t_para;

