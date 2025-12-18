/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tonemap_internal.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 17:14:03 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/17 17:35:07 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TONEMAP_INTERNAL_H
# define TONEMAP_INTERNAL_H

# include "render.h"

# ifndef TONESIZE
#  define TONESIZE 512
# endif

# define TM_PERCENTILE 0.95f
# define TM_KEY 0.54f
# define TM_LUM_MIN 0.0004f
# define TM_LUM_MAX 10000.0f
# define TM_EPS 0.000001f

float	tm_luminance(const t_vec3f *color_lin);
void	tm_hist_clear(float *hist);
int		tm_bin_from_lum(float lum, float log_min, float inv_log_range);
float	tm_hist_total(const float *hist);
float	tm_lum_from_bin(int bin, float lum_min, float lum_max);
float	tm_percentile_lum(const float *hist, float percentile);
void	tm_build_hist_sampled(t_mgr *mgr, int width, int height);

float	tm_exposure_from_lum(float lum_ref, float key);
float	tm_filmic_exp(float lum, float exposure);
void	tm_apply_pixel(t_mgr *mgr, int index, float exposure);
void	tm_apply_full_image(t_mgr *mgr, int width, int height, float exposure);

#endif