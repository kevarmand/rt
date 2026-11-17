#ifndef POSTPROCESS_H
# define POSTPROCESS_H

# define POST_LUT_SIZE 16384
# define POST_GAMMA_MIN 10
# define POST_GAMMA_MAX 50
# define POST_GAMMA_DEFAULT 20
# define POST_EV_MIN (-30)
# define POST_EV_MAX 30
# define POST_SETTINGS_MASK 0x3fffu
# define POST_GENERATION_STEP 0x4000u

/* Low 14 bits: settings. High bits: publication generation. */
static inline unsigned int	post_pack(int enabled, int automatic,
		int gamma_step, int ev_step)
{
	return ((unsigned int)enabled | ((unsigned int)automatic << 1)
		| ((unsigned int)gamma_step << 2)
		| ((unsigned int)(ev_step - POST_EV_MIN) << 8));
}

static inline unsigned int	post_default(void)
{
	return (post_pack(1, 1, POST_GAMMA_DEFAULT, 0));
}

static inline int	post_gamma(unsigned int settings)
{
	return ((settings >> 2) & 63u);
}

static inline int	post_ev(unsigned int settings)
{
	return ((int)((settings >> 8) & 63u) + POST_EV_MIN);
}

#endif
