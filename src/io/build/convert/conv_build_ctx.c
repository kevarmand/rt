#include "convert.h"
#include "io.h"
#include "errors.h"

void print_ctx(t_conv_ctx *cx);

int	conv_build_ctx(t_scene_parsed *parsed, t_conv_ctx *ctx)
{
	int	status;

	conv_count_all(parsed, ctx);
	//on va afficher les counts pour debug
	print_ctx(ctx);
	status = conv_cameras(parsed, ctx);
	print_ctx(ctx);
	if (status == SUCCESS)
		status = conv_lights(parsed, ctx);

	print_ctx(ctx);
	if (status == SUCCESS)
		status = conv_primitives(parsed, ctx);
	return (status);
}
