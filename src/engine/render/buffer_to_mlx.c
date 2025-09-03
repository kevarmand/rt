#include "render.h"
#include "rt.h"


void	buffer_to_mlx_image(t_image *img, const int *buf, int src_pitch_px)
{
	int				x;
	int				y;
	unsigned char	*base;
	unsigned char	*dst;
	const int		*src;
	int				bytespp;

	bytespp = img->bpp / 8;
	base = (unsigned char *)img->addr_ptr;
	y = 0;
	while (y < img->height)
	{
		src = buf + ((size_t)y * (size_t)src_pitch_px) * 3;
		dst = base + (size_t)y * (size_t)img->size_l;
		x = 0;
		while (x < img->width)
		{
			dst[0] = (unsigned char)src[2];
			dst[1] = (unsigned char)src[1];
			dst[2] = (unsigned char)src[0];
			if (bytespp >= 4)
				dst[3] = (unsigned char)255;
			src += 3;
			dst += bytespp;
			x++;
		}
		y++;
	}
}
