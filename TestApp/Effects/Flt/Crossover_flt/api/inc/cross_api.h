#ifndef __CROSS_API_H__
#define __CROSS_API_H__

#include "cross_flt_control.h"
#include "cross_flt_process.h"
#include "params_id.h"
#include "colors.h"

#include <string.h>
#include <stdio.h>
#include <math.h>

#include "cross_flt.h"

typedef struct coef_t{
    cross_coef_t    cross_coef;
    coef_type       gain_c[NUM_OF_BAND];
}coef_t;

typedef struct prm_t{
    cross_prm_t     cross_coef;
    double          gain_dB[NUM_OF_BAND];
}prm_t;

typedef struct states_s{
    cross_states_t Left;
    cross_states_t Right;
}states_t;

int32_t cross_coeff_calc(cross_prm_t *_prm, coef_type *_coeffs);

#endif