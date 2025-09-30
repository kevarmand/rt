#ifndef IO_H
# define IO_H

# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <unistd.h>
# include <string.h>
# include "type.h"
# include "libft.h"


# define BUFFER_SIZE 1024

typedef enum e_elem_role
{
	ELEM_ROLE_NORMAL,
	ELEM_ROLE_PRIMARY,
	ELEM_ROLE_EXTRA
}	t_elem_role;

typedef struct s_pars_state {
	int			line_no;
	int			depth;
	const char	*line;
	int			pos;
}	t_pars_state;

typedef struct s_scene t_scene;

typedef struct s_tok {
	const char	*start;
	int			len;
}	t_tok;

typedef enum e_pmask
{
	PRESENCE_CAM = 1u << 0,
	PRESENCE_AMBIENT = 1u << 1,
	PRESENCE_RESOLUTION = 1u << 2,
	PRESENCE_OBJECT = 1u << 3,
	PRESENCE_LIGHT = 1u << 4
}	t_pmask;

typedef struct s_param
{
	int		res_width;
	int		res_height;
	float	brightness;
	float	color[3];
}	t_param;

typedef struct s_scene_parsed
{
	t_list	*objects;
	t_list	*cameras;
	t_list	*lights;
	t_param	globals;
	t_pmask	presence_mask;
}	t_scene_parsed;

typedef struct s_element_options
{
	float		ior;
	float		reflection;
	float		refraction;
	float		shininess;
	float		diffuse_weight;
	float		specular_weight;
	float		ambient_occlusion;
	//etc ...
}	t_element_options;

typedef enum e_element_type {
	ELEM_NONE,
	ELEM_SPHERE,
	ELEM_PLANE,
	ELEM_CYLINDER,
	ELEM_TRIANGLE,
	ELEM_CAMERA,
	ELEM_LIGHT,
	ELEM_AMBIENT
}	t_element_type;

typedef struct s_parsed_sphere
{
	float			center[3];
	float			diameter;
	int				rgb[3];
}	t_parsed_sphere;

typedef struct s_parsed_plane
{
	float			origin[3];
	float			normal[3];
	int				rgb[3];
}	t_parsed_plane;

typedef struct s_parsed_cylinder
{
	float			origin[3];
	float			axis[3];
	float			diameter;
	float			height;
	int				rgb[3];
}	t_parsed_cylinder;

typedef struct s_parsed_triangle
{
	float			vertex1[3];
	float			vertex2[3];
	float			vertex3[3];
	int				rgb[3];
}	t_parsed_triangle;

typedef struct s_parsed_camera
{
	float			position[3];
	float			orientation[3];
	int				fov;
}	t_parsed_camera;

typedef struct s_parsed_light
{
	float			position[3];
	float			brightness;
	int				rgb[3];
}	t_parsed_light;

typedef struct s_parsed_element
{
	t_element_type			type;
	t_element_options		options;
	union {
		t_parsed_sphere		sphere;
		t_parsed_plane		plane;
		t_parsed_cylinder	cylinder;
		t_parsed_triangle	triangle;
		t_parsed_camera		camera;
		t_parsed_light		light;
	}	data;
}	t_parsed_element;


/**
 * @brief Parse a scene from a file and populate the provided t_scene structure.
 * @param filename The path to the scene file to be parsed. (no_null)
 * @param scene A pointer to a t_scene structure to be populated 
 * with the parsed data.
 * @return Returns 0 on success, or a non-zero error code on failure.
 */
int	pars_scene(const char *filename, t_scene *scene);

/***
 * @brief Initialize the parsing context state.
 * @param st Pointer to the parsing state structure to initialize. (no_null)
 */
void	pars_ctx_init(t_pars_state *st);

/***
 * @brief Initialize a t_scene_parsed structure with default values.
 * @param scene Pointer to the t_scene_parsed structure to initialize. (no_null)
 * @return Returns SUCCESS on successful initialization.
 */
int	pars_init_scene(t_scene_parsed *scene);

/**
 * @brief Parse a scene from an open file descriptor into a temporary builder.
 * @param fd    Open file descriptor to the .rt scene (read-only). (no_null)
 * @param scene Builder receiving parsed elements (lists, presence mask). (no_null)
 * @return SUCCESS on success, otherwise a parsing/I-O error code.
 * @note Does not close @fd. Preconditions are guaranteed by the caller.
 */
int	pars_scene_fd(int fd, t_scene_parsed *scene);


/**
 * @brief Parse a single line: read first word span, dispatch by tag.
 * @note  Braces '{}' unsupported yet (reserved for later).
 */
int	pars_line(const char *line, t_scene_parsed *scene, t_pars_state *st);

/**
 * @brief Get next whitespace-delimited token from st->line as a span.
 * @param st  Parsing state with current line and position. (no_null)
 * @param out Output token span (start pointer and length). (no_null)
 * @return 1 if found, 0 if EOL or comment start.
 */
int	pars_next_tok(t_pars_state *st, t_tok *out);


/***
 * @brief Dispatch a token to the appropriate parsing function based on its tag.
 * @param tag   The token to be dispatched. (no_null)
 * @param st    The current parsing state. (no_null)
 * @param scene The scene being constructed. (no_null)
 * @return Returns 0 on success, or a non-zero error code on failure.
 */
int	pars_dispatch_tok(t_tok tag, t_pars_state *st, t_scene_parsed *scene);

/***
 * @brief Compare a token with a literal string for equality.
 * @param t   The token to compare. (no_null)
 * @param lit The literal string to compare against. (no_null)
 * @return Returns 1 if the token matches the literal, otherwise returns 0.
 */
int	pars_tok_eq(t_tok t, const char *lit);

/* ************************************************************************** */
/*                             UTILS - PARSING                                */
/* ************************************************************************** */

 /***
 * @brief Scan a floating-point number from a token span.
 * @param tok	The token span containing the float representation. (no_null)
 * @param out_value Pointer to store the parsed float value. (no_null)
 * @return SUCCESS on successful parse, or ERR_PARSE_FLOAT on failure.
 * @note The function handles optional leading '+' or '-' signs, integer
 * and fractional parts, and ensures the value is within a reasonable range.
 * It does not handle scientific notation (e.g., '1.23e4').
 */
int	scan_float(t_tok tok, float *out_value);

/**
 * @brief Parse a color from a token in the format "R,G,B".
 * @param tok      The input token containing the color. (no_null)
 * @param out_rgb  Output array to store the parsed RGB components. (no_null)
 * @return Returns 0 on success, or a non-zero error code on failure.
 * @note Each RGB component must be an integer in the range 0-255.
 */
int	scan_color(t_tok tok, int out_rgb[3]);

/***
 * Scan a 3D vector from the token stream.
 * @param tok  Input token containing the vector in "X,Y,Z" format. (no_null)
 * @param vec  Output array to store the parsed vector components. (no_null)
 * @return Returns 0 on success, or a non-zero error code on failure.
 * @note The function parses the vector components, checks if they are within
 * the range [-1, 1], and normalizes the vector to unit length if necessary.
 */
int	scan_vec3(t_tok tok, float vec[3]);

/***
 * @brief Scan a 3D point from a token span.
 * @param tok The token span containing the point representation. (no_null)
 * @param out_vec Pointer to store the parsed 3D point (array of 3 floats).
 *  (no_null)
 * @return SUCCESS on successful parse, or ERR_PARSE_FLOAT on failure.
 * @note The function expects the point to be in the format "x,y,z"
 * where x, y, and z are floating-point numbers. 
 * It does not handle spaces or other delimiters.
 */
int	scan_point(t_tok tok, float out_vec[3]);

/* ************************************************************************** */
/*                        UTILS - OPTIONS PARSING                             */
/* ************************************************************************** */

/***
 * Parse options for an element from the token stream.
 * @param st      The current parsing state. (no_null)
 * @param options Pointer to the element options structure to populate. (no_null)
 * @return Returns 0 on success, or a non-zero error code on failure.
 * @note The function reads tokens until no more valid options are found.
 * Each option is expected to be in the format "key=value".
 */
int	pars_options(t_pars_state *st, t_element_options *options);

/**
 * @brief Split a token of the form "key=value" into key and value tokens.
 * And dispatch to the appropriate option handler.
 * @param tok        The input token to split. (no_null)
 * @param key_out    Output token for the key part. (no_null)
 * @param value_out  Output token for the value part. (no_null)
 * @return 0 on success, or ERR_PARSE_FLOAT if the format is invalid.
 * @note Both output tokens point into the original token's string.
 */
int	scan_option(t_tok tok, t_element_options *opts);

int	scan_opt_ior(t_tok tok, t_element_options *opts);
int	scan_opt_refraction(t_tok tok, t_element_options *opts);
int	scan_opt_reflection(t_tok tok, t_element_options *opts);
int	scan_opt_shininess(t_tok tok, t_element_options *opts);
int	scan_opt_diffuse(t_tok tok, t_element_options *opts);
int	scan_opt_specular(t_tok tok, t_element_options *opts);
int	scan_opt_ambient(t_tok tok, t_element_options *opts);


/* ************************************************************************** */
/*                             PARSING - OBJECT                               */
/* ************************************************************************** */

/**
 * @brief Dispatch the parsing of a token to the appropriate handler function.
 * @param tag The token representing the element type to parse. (no_null)
 * @param st  The current parsing state. (no_null)
 * @param scene The scene structure to populate with parsed data. (no_null)
 * @return Returns 0 on success, or a non-zero error code on failure.
 * @note If the token does not match any known element type,
 * an error code is returned.
 */
int	pars_dispatch_tok(t_tok tag, t_pars_state *st, t_scene_parsed *scene);

//For each object type :
// -take the mandatory parameters from the token stream
// -call pars_options() to take optional parameters

int	pars_sphere(t_pars_state *st, t_scene_parsed *scene);
int	pars_plane(t_pars_state *st, t_scene_parsed *scene);
int	pars_cylinder(t_pars_state *st, t_scene_parsed *scene);
int	pars_triangle(t_pars_state *st, t_scene_parsed *scene);
int	pars_camera(t_pars_state *st, t_scene_parsed *scene,
			t_elem_role role);
int	pars_light(t_pars_state *st, t_scene_parsed *scene,
			t_elem_role role);
int	pars_ambient(t_pars_state *st, t_scene_parsed *scene);
int	pars_resolution(t_pars_state *st, t_scene_parsed *scene);

/* ************************************************************************** */
/*                        SCENE BUILDER - OBJECTS                             */
/* ************************************************************************** */

/***
 * @brief Register a parsed element into the scene structure.
 * @param scene The scene structure to populate. (no_null)
 * @param elem  The parsed element to register. (no_null)
 * @param role  The role of the element (normal, primary, extra).
 * @return Returns 0 on success, or a non-zero error code on failure.
 * @note The function adds the element to the appropriate list in the scene
 * based on its type and role. It handles memory allocation and ensures
 * that required elements are present.
 */
int	pars_register_element(t_scene_parsed *scene,
			const t_parsed_element *elem, t_elem_role role);

typedef int	(*t_opt_handler)(t_tok, t_element_options *);

typedef struct s_opt_entry {
	const char		*key;
	t_opt_handler	fn;
}	t_opt_entry;
#endif

