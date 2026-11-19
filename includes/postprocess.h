#ifndef POSTPROCESS_H
# define POSTPROCESS_H

# define POST_LUT_SIZE 16384
# define POST_GAMMA_MIN 5
# define POST_GAMMA_MAX 80
# define POST_GAMMA_DEFAULT 20
# define POST_EV_MIN (-60)
# define POST_EV_MAX 60
# define POST_SETTINGS_MASK 0x3fffu
# define POST_GENERATION_STEP 0x4000u

/* Low 14 bits: settings. High bits: publication generation. */
static inline unsigned int	post_pack(int gamma_step, int ev_step)
{
	return ((unsigned int)gamma_step
		| ((unsigned int)(ev_step - POST_EV_MIN) << 7));
}

static inline unsigned int	post_default(void)
{
	return (post_pack(POST_GAMMA_DEFAULT, 0));
}

static inline int	post_gamma(unsigned int settings)
{
	return (settings & 127u);
}

static inline int	post_ev(unsigned int settings)
{
	return ((int)((settings >> 7) & 127u) + POST_EV_MIN);
}

#endif
