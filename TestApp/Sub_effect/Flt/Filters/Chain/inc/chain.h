#ifndef __CHAIN_H__
#define __CHAIN_H__

#include "eq.h"

#define EQ_BANDS        10

typedef struct eq_params_s{
    eq_params_t eq1;
}chain_params_t;

typedef struct chain_states_s{
    eq_states_t eq1;
}chain_states_t;

typedef struct chain_coeffs_s{
    eq_coeffs_t eq1;
}chain_coeffs_t;

#endif