#include "io.h"
#include "rt.h"
#include "error_codes.h"
#include "scene.h"


/***
 * @brief Get the idx of a material with given options, or create a new one if it doesn't exist.
*/
int	mat_getidx(t_vector *materials, t_hashmap *mat_map, const t_element_options *opts)
{
	//1- create a unique key for the options
	//2- check if the key exists in the hashmap
	//3- if it exists, return the index
	//4- if it doesn't exist, create a new material with these options, add it to the vector and hashmap, return the new index

	
}