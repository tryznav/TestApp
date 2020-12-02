#ifndef __CHAIN_H__
#define __CHAIN_H__

#include "eq.h"
#include "compressors.h"

typedef struct chain_params_s{
    eq_params_t                 eq1;
    compressor_4band_params_t   compressor;
}chain_params_t;

typedef struct chain_states_s{
    eq_states_t                 eq1;
    compressor_4band_states_t   compressor;
}chain_states_t;

typedef struct chain_coeffs_s{
    eq_coeffs_t                 eq1;
    compressor_4band_coeffs_t   compressor;
}chain_coeffs_t;

#endif