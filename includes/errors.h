/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 21:45:42 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 07:29:38 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_H
# define ERRORS_H

// # define ERROR				-1
// # define ERR_INTERNAL		-1
// # define ERR_THREAD			-2
// # define ERR_ARG			-2
// # define ERR_EXT			-3
// # define ERR_OPEN			-4
// # define ERR_MIN			-5
// # define ERR_MALLOC			-100
// # define ERR_ALLOC			-100

// # define ERR_MLX			-6
// # define ERR_BMP			-7

// # define ERR_DOUBLE			-200
// # define ERR_COLOR			-201

// # define ERR_A				-210
// # define ERR_R				-211
// # define ERR_C				-212
// # define ERR_L				-213
// # define ERR_SP				-214
// // # define ERR_PL				-215
// // # define ERR_TR				-216
// // # define ERR_CY				-217
// // # define ERR_SQ				-218
// // # define ERR_R_REDEF		-219
// // # define ERR_A_REDEF		-220

// // # define ERR_PARS_TYPE		-221
// # define ERR_CTX_UNBALANCED	-300
// # define ERR_CTX_UNSUPPORTED -301
// # define ERR_PARSE_FLOAT	-400

// typedef enum e_err
// {
// 	SUCCESS = 0,
// 	ERR_GENERIC = -1,
// 	ERR_THREAD_CREATE = -2,
// 	ERR_INVALID_ARGS = -100,
// 	ERR_PARS = -200,
// 	ERR_PARS_COLOR = -201,
// 	ERR_PARS_VEC3 = -202,
// 	ERR_PARS_POS = -203,
// 	ERR_PARS_DIR = -204,
// 	ERR_PARS_FOV = -205,
// 	ERR_PARS_DIAM = -206,
// 	ERR_PARS_AMBIANT = -207,
// 	ERR__COUNT
// }	t_err;

typedef enum e_err
{
	SUCCESS = 0,
	ERR_INTERNAL = -1,
	ERR_THREAD = -2,
	ERR_MALLOC = -3,
	ERR_MLX = -4,
	ERR_OPEN = -5,
	ERR_PARS = -6,
}	t_err;

# define PERR_MALLOC		"Memory allocation failed."
# define PERR_MLX_INIT		"Failed to initialize MLX."
# define PERR_MLX_IMG		"Failed to create MLX image."
# define PERR_MLX_WIN		"Failed to create MLX window."

# define PERR_MALLOC_BIT	"Memory allocation failed (bitmap)."
# define PERR_M_HASH_TEXT	"Failed to create hashmap for textures."
# define PERR_M_HASH_BUMP	"Failed to create hashmap for bumpmaps."
# define PERR_M_HASH_MAT	"Failed to create hashmap for materials."
# define PERR_M_HASH_INSERT	"Failed to insert into hashmap."

# define PERR_M_LIST		"Failed to allocate list node."
# define PERR_M_LIST_ELEM	"Failed to allocate list element."

# define PERR_M_VEC_MAT		"Failed to initialize material vector."
# define PERR_M_VEC_SURF	"Failed to initialize surface map vector."
# define PERR_M_VEC_CAM		"Failed to initialize camera vector."
# define PERR_M_VEC_LIGHT	"Failed to initialize light vector."
# define PERR_M_VEC_PLANE	"Failed to initialize plane vector."
# define PERR_M_VEC_OBJ		"Failed to initialize object vector."

# define PERR_M_TEX_PIXELS	"Failed to allocate texture pixels."

//render init errors

# define PERR_M_MAIN_BUFFERS	"Failed to initialize main render buffers."
# define PERR_M_MAILBOX		"Failed to initialize mailbox buffers."
# define PERR_M_WORKERS		"Failed to initialize worker threads."
# define PERR_M_WORKER_TILES	"Failed to initialize worker tile buffers."
# define PERR_M_TILESET		"Failed to initialize tileset."

//thread errors

# define PERR_M_THREAD_ARGS	"Failed to allocate thread arguments."
# define PERR_M_THREAD_CREATE	"Failed to create thread."

# define ERR_EXT_XPM		"Texture file is not an XPM."
# define PERR_MLX_IMGXPM	"Failed to load XPM image."

# define PERR_M_ARRAY		"Failed to allocate array."
# define PERR_M_ARRAY_TEXT	"Failed to allocate texture array."
# define PERR_M_ARRAY_BUMP	"Failed to allocate bumpmap array."

# define PERR_M_VEC_PUSH	"Failed to push element to vector."

# define PERR_OPEN			"Failed to open scene file."

# define PERR_BVH_INIT		"Failed to initialize BVH."

# define PERR_CAM_REDEF		"Multiple primary cameras defined."
# define PERR_LIGHT_REDEF	"Multiple primary lights defined."
# define PERR_AMBIENT_REDEF	"Multiple ambient light definitions."
# define PERR_RESOLUTION_REDEF	"Multiple resolution definitions."

# define PERR_MLX_NEW_IMG	"Failed to create new MLX image."


//DISPLAY ERRORS

# define PERR_M_DISPLAY_FRAME		"Failed to allocate display frames."
# define PERR_M_DISPLAY_FRAME_PIX	"Failed to allocate display frame pixels."




/***
 * print an error message corresponding to the error code
 * @param err_code the error code
 * @param msg the message to print
 * @return the error code
 */
int	perr(int status, const char *msg);

/***
 * print an error message with line number
 * @param err_code the error code
 * @param line the line number
 * @return the error code
 */
int	perr_line(int err, const char *line, int line_no);

#endif