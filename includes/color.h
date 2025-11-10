/* includes/color.h */
#ifndef COLOR_H
# define COLOR_H
# include <stddef.h>
# include "vector.h"

float	srgb8_to_linear(int c);
int		linear_to_srgb8(float x);
void	rgb8_to_linear_vec(const int rgb[3], t_vec3f *out);
void	linear_to_rgb8_vec(const t_vec3f *lin, int rgb_out[3]);

#endif
