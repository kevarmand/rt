#include "convert.h"
#include "io.h"
#include "errors.h"

int	conv_build_ctx(t_scene_parsed *parsed, t_conv_ctx *ctx)
{
	int	status;

	conv_count_all(parsed, ctx);
	status = conv_cameras(parsed, ctx);
	if (status == SUCCESS)
		status = conv_light(parsed, ctx);
	if (status == SUCCESS)
		status = conv_primitives(parsed, ctx);
	return (status);
}
