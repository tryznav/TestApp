#ifndef __EQ_H__
#define __EQ_H__

#include "filters.h"

#define EQ_BANDS        10

typedef struct eq_params_s{
    // biquad_params_t           bands[EQ_BANDS];
    biquad_cascade4_params_t  cascade4[2];
    biquad_cascade2_params_t  cascade2;
}eq_params_t;

typedef struct eq_states_s{
    biquad_cascade4_states_t  cascade4[2];
    biquad_cascade2_states_t  cascade2;
}eq_states_t;

typedef struct eq_coeffs_s{
    biquad_cascade4_coeffs_t  cascade4[2];
    biquad_cascade2_coeffs_t  cascade2;
}eq_coeffs_t;

#endif