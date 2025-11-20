#ifndef ERRORS_H
#define ERRORS_H


# define ERROR				-1

# define ERR_ARG			-2
# define ERR_EXT			-3
# define ERR_OPEN			-4
# define ERR_MIN			-5
# define ERR_MALLOC			-100
# define ERR_ALLOC			-100

# define ERR_MLX			-6
# define ERR_BMP			-7

# define ERR_DOUBLE			-200
# define ERR_COLOR			-201

# define ERR_A				-210
# define ERR_R				-211
# define ERR_C				-212
# define ERR_L				-213
# define ERR_SP				-214
# define ERR_PL				-215
# define ERR_TR				-216
# define ERR_CY				-217
# define ERR_SQ				-218
# define ERR_R_REDEF		-219
# define ERR_A_REDEF		-220
// # define ERR_PARS_TYPE		-221

# define ERR_CTX_UNBALANCED	-300
# define ERR_CTX_UNSUPPORTED -301

# define ERR_PARSE_FLOAT	-400

typedef enum e_err
{
	SUCCESS = 0,
	/* génériques */
	ERR_GENERIC = -1,
	ERR_INVALID_ARGS = -100,
	ERR_PARS = -200,
	/* parsing	 */
	ERR_PARS_COLOR = -201,
	ERR_PARS_VEC3 = -202,
	ERR_PARS_POS = -203,
	ERR_PARS_DIR = -204,
	ERR_PARS_FOV = -205,
	ERR_PARS_DIAM = -206,
	ERR_PARS_AMBIANT = -207,

	ERR__COUNT /* doit rester le dernier pour dimensionner g_errmsg */
}	t_err;


/***
 * print an error message corresponding to the error code
 * @param err_code the error code
 * @param msg the message to print
 * @return the error code
 */
int	print_error(t_err err_code, const char *msg);

/***
 * print an error message with file and line number
 * @param err the error code
 * @param file the file name
 * @param line the line number
 * @return the error code
 */
int	error_at(int err, const char *file, int line);

#endif