#ifndef __TEST_SIG_GEN_H__
#define __TEST_SIG_GEN_H__

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>

#include "colors.h"

#pragma pack(push,1)

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

typedef struct tsig_square_stat_s
{
    uint32_t    sample_increment;
    float       sign_value;
}tsig_square_stat_t;


float   *tsig_delta_st  (uint32_t sample_rate, uint32_t length_sample, void *params, void* states, void *audio);
float   *tsig_step_st   (uint32_t sample_rate, uint32_t length_sample, void *params, void* states, void *audio);
float   *tsig_square_st (uint32_t sample_rate, uint32_t length_sample, void *params, void* states, void *audio);
float   *tsig_wnoise_st (uint32_t sample_rate, uint32_t length_sample, void *params, void* states, void *audio);
float   *tsig_sine      (uint32_t sample_rate, uint32_t length_sample, void *params, void* states, void *audio);

#endif /*test_sig_gen.h*/