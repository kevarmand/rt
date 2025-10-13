#include "io.h"
#include "rt.h"
#include "error_codes.h"
#include "scene.h"

#define KEY_SIZE 128

static int32_t	quantize_option(float value)
{
	float		scale;
	float		scaled;
	int32_t		result;

	scale = 10000.0f;
	scaled = roundf(value * scale);
	result = (int32_t)scaled;
	return (result);
}

static void	write_hex8(char *dst, uint32_t value)
{
	static const char	*digits = "0123456789ABCDEF";
	int					i;
	int					shift;

	i = 0;
	while (i < 8)
	{
		shift = (7 - i) * 4;
		dst[i] = digits[(value >> shift) & 0xF];
		i++;
	}
}

static void	write_float_to_key(float value, char *key, int *index)
{
	int32_t	q;

	q = quantize_option(value);
	write_hex8(key + *index, (uint32_t)q);
	*index += 8;
}

void	generate_option_key(const t_element_options *opts, char *key)
{
	int	write_index;

	write_index = 0;
	write_float_to_key(opts->ior, key, &write_index);
	if (write_index < KEY_SIZE - 8)
		write_float_to_key(opts->reflection, key, &write_index);
	if (write_index < KEY_SIZE - 8)
		write_float_to_key(opts->refraction, key, &write_index);
	if (write_index < KEY_SIZE - 8)
		write_float_to_key(opts->shininess, key, &write_index);
	if (write_index < KEY_SIZE - 8)
		write_float_to_key(opts->diffuse_weight, key, &write_index);
	if (write_index < KEY_SIZE - 8)
		write_float_to_key(opts->specular_weight, key, &write_index);
	if (write_index < KEY_SIZE - 8)
		write_float_to_key(opts->ambient_occlusion, key, &write_index);
	key[write_index] = '\0';
}