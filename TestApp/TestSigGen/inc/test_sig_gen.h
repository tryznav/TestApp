#ifndef __TEST_SIG_GEN_H__
#define __TEST_SIG_GEN_H__

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <errno.h>
#include <string.h>

#define CHIRP_LINEAR            1
#define CHIRP_LOGARITM          2

#include "colors.h"

#define M_PI 3.14159265358979323846
#pragma pack(push,1)
// extern int errno;

typedef struct chanels_s{
    float Left;
    float Right;
}chanels_t;
#pragma pack(pop)

typedef struct tsig_wnoise_prm_s
{
    uint32_t amplitude;
}tsig_wnoise_prm_t;

typedef struct tsig_square_prm_s
{
    uint32_t period_ms;
}tsig_square_prm_t;

typedef struct tsig_sin_prm_s
{
    uint32_t    freq;
}tsig_sin_prm_t;

typedef struct tsig_chirp_prm_s
{
    uint32_t    chirp_type_id;
    uint32_t    freq_start;
    uint32_t    freq_stop;
}tsig_chirp_prm_t;

typedef struct tsig_l_sw_sin_prm_s
{
    uint32_t    amp_start_dB;
    uint32_t    amp_stop_dB;
    uint32_t    freq;
}tsig_l_sw_sin_prm_t;

typedef struct tsig_square_stat_s
{
    uint32_t    half_period_sample;
    uint32_t    sample_increment;
    float       sign_value;
}tsig_square_stat_t;

typedef struct tsig_delta_stat_s
{
    uint32_t    ex;
}tsig_delta_stat_t;

typedef struct tsig_sin_stat_s
{
    uint32_t    sample_in_period;
    uint32_t    sample_increment;
}tsig_sin_stat_t;

typedef struct tsig_chirp_stat_s
{
    float       freq_increment_num;
    float       freq_increment;
    uint32_t    sample_increment;
}tsig_chirp_stat_t;

typedef struct tsig_l_sw_sin_stat_s
{
    float       amp_increment_num;
    float       amp_increment;
    uint32_t    sample_increment;
    double      sin_coef;
}tsig_l_sw_sin_stat_t;


void *tsig_wnoise_init_states   (uint32_t sample_rate, uint32_t length_sample, void* states, void const *params);
void *tsig_step_init_states     (uint32_t sample_rate, uint32_t length_sample, void* states, void const *params);
void *tsig_square_init_states   (uint32_t sample_rate, uint32_t length_sample, void* states, void const *params);
void *tsig_sin_init_states      (uint32_t sample_rate, uint32_t length_sample, void* states, void const *params);
void *tsig_lsw_sin_init_states  (uint32_t sample_rate, uint32_t length_sample, void* states, void const *params);

int32_t tsig_gen_delta_st   (uint32_t sample_rate, uint32_t length_sample, float amplitude_coef, void const *params, void* states, void *audio);
int32_t tsig_gen_step_st    (uint32_t sample_rate, uint32_t length_sample, float amplitude_coef, void const *params, void* states, void *audio);
int32_t tsig_gen_square_st  (uint32_t sample_rate, uint32_t length_sample, float amplitude_coef, void const *params, void* states, void *audio);
int32_t tsig_gen_wnoise_st  (uint32_t sample_rate, uint32_t length_sample, float amplitude_coef, void const *params, void* states, void *audio);
int32_t tsig_gen_sine       (uint32_t sample_rate, uint32_t length_sample, float amplitude_coef, void const *params, void* states, void *audio);
int32_t tsig_gen_chirp_st   (uint32_t sample_rate, uint32_t length_sample, float amplitude_coef, void const *params, void* states, void *audio);
int32_t tsig_gen_lsw_sin_st (uint32_t sample_rate, uint32_t length_sample, float amplitude_coef, void const *params, void* states, void *audio);

#endif /*test_sig_gen.h*/