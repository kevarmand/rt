#include "scene.h"
#include "convert.h"
#include "io.h"
#include "error_codes.h"
#include <stdlib.h>
#include <string.h>

int	pars_convert_scene(const t_scene_parsed *parsed, t_scene *scene)
{
	t_conv_ctx cx;
	int        err;

	/* Préconditions garanties par le parsing/init : 
	   - resolution > 0
	   - orientations normalisées
	   - options valides
	   - listes non NULL
	*/

	cx.width = /* parsed->globals.res_width */;
	cx.height = /* parsed->globals.res_height */;
	cx.aspect = (float)cx.width / (float)cx.height;

	conv_count_all(parsed, &cx);

	err = conv_alloc_scene(scene, &cx);
	if (err != SUCCESS)
		return (err);

	/* Dédup/émission dans l’ordre qui te convient */
	err = conv_build_textures(parsed, scene, &cx);
	if (err == SUCCESS)
		err = conv_build_materials(parsed, scene, &cx);
	if (err == SUCCESS)
		err = conv_emit_objects(parsed, scene, &cx);
	if (err == SUCCESS)
		err = conv_emit_primitives(parsed, scene, &cx);
	if (err == SUCCESS)
		err = conv_emit_planes(parsed, scene, &cx);
	if (err == SUCCESS)
		err = conv_emit_lights(parsed, scene, &cx);
	if (err == SUCCESS)
		err = conv_build_cameras(parsed, scene, &cx);

	/* BVH plus tard : si activé, ici */
	/* if (err == SUCCESS) err = conv_build_bvh(scene); */

	return (err);
}