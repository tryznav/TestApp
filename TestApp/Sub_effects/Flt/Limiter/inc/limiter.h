#ifndef __COMPRESSORS_H__
#define __COMPRESSORS_H__

#ifndef  M_e
    #define M_e    2.71828182846
#endif

#include "crossovers.h"

typedef struct compressor_4band_params_s{
    crossover4b_params_t        crossover;
    double                      tau_attack[4];
    double                      tau_release[4];
    double                      tau_env_attack[4];
    double                      tau_env_release[4];
    double                      makeUpGain[4];
    double                      threshold[4];
    double                      ratio[4];
    uint32_t                    enable[4];
    double                      sample_rate;
}compressor_4band_params_t;

typedef struct compressor_4band_states_s{
    crossover4b_states_t        crossover;
    vfloat8_t                   env;
    vfloat8_t                   g_s;
}compressor_4band_states_t;

typedef struct compressor_4band_coeffs_s{
    crossover4b_coeffs_t        crossover;
    vfloat8_t                   e_release;
    vfloat8_t                   e_release_m;
    vfloat8_t                   a_release;
    vfloat8_t                   a_release_m;
    vfloat8_t                   e_attack;
    vfloat8_t                   e_attack_m;
    vfloat8_t                   a_attack;
    vfloat8_t                   a_attack_m;
    vfloat8_t                   threshold;
    vfloat8_t                   ratio;
    vfloat8_t                   makeUpGain;
    vfloat8_t                   enable_mask;
}compressor_4band_coeffs_t;

#endif