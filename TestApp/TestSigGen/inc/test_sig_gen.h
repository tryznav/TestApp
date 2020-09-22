#ifndef __TEST_SIG_GEN_H__
#define __TEST_SIG_GEN_H__

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <errno.h>
#include <string.h>




#define TSIG_DELTA               1
#define TSIG_STEP                2
#define TSIG_SQUARE              3
#define TSIG_WNOISE              4
#define TSIG_SINE                5
#define TSIG_CHIRP_LINEAR        6
#define TSIG_CHIRP_LOGARITM      7
#define TSIG_LEVEL_SWEEP         8

#define  MAX_PRM_SIZE           32
#include "colors.h"

// #define _USE_MATH_DEFINES
#define M_PI 3.14159265358979323846

// #pragma pack(push,1)
// extern int errno;

typedef struct chanels_s{
    float Left;
    float Right;
}chanels_t;
// #pragma pack(pop)

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

typedef struct tsig_lsw_sin_prm_s
{
    float       amp_start_dB;
    float       amp_stop_dB;
    float       freq;
}tsig_lsw_sin_prm_t;

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

typedef struct tsig_lsw_sin_stat_s
{
    float       amp_increment_num;
    float       amp_increment;
    uint32_t    sample_increment;
    double      sin_coef;
}tsig_lsw_sin_stat_t;

void *tsig_delta_init_states    (uint32_t sample_rate, uint32_t length_sample, void const *params);
void *tsig_wnoise_init_states   (uint32_t sample_rate, uint32_t length_sample, void const *params);
void *tsig_step_init_states     (uint32_t sample_rate, uint32_t length_sample, void const *params);
void *tsig_square_init_states   (uint32_t sample_rate, uint32_t length_sample, void const *params);
void *tsig_sin_init_states      (uint32_t sample_rate, uint32_t length_sample, void const *params);
void *tsig_lsw_sin_init_states  (uint32_t sample_rate, uint32_t length_sample, void const *params);
void *tsig_chirp_init_states    (uint32_t sample_rate, uint32_t length_sample, void const *params);

int32_t tsig_gen_delta_st   (uint32_t sample_rate, uint32_t length_sample, float amplitude_coef, void const *params, void* states, void *audio);
int32_t tsig_gen_step_st    (uint32_t sample_rate, uint32_t length_sample, float amplitude_coef, void const *params, void* states, void *audio);
int32_t tsig_gen_square_st  (uint32_t sample_rate, uint32_t length_sample, float amplitude_coef, void const *params, void* states, void *audio);
int32_t tsig_gen_wnoise_st  (uint32_t sample_rate, uint32_t length_sample, float amplitude_coef, void const *params, void* states, void *audio);
int32_t tsig_gen_sine_st    (uint32_t sample_rate, uint32_t length_sample, float amplitude_coef, void const *params, void* states, void *audio);
int32_t tsig_gen_chirp_st   (uint32_t sample_rate, uint32_t length_sample, float amplitude_coef, void const *params, void* states, void *audio);
int32_t tsig_gen_lsw_sin_st (uint32_t sample_rate, uint32_t length_sample, float amplitude_coef, void const *params, void* states, void *audio);

#endif /*test_sig_gen.h*/