#ifndef __CROSS_FLT_H__
#define __CROSS_FLT_H__

#include "cross_flt_control.h"
#include "cross_flt_process.h"
#include "params_id.h"

#include "flt_arithmetic.h"
#include "colors.h"

#include <string.h>
#include <stdio.h>
#include <math.h>

#define TAP_NUM         256

/*******************************************************************************
 * the bandwidth in octaves (between -3 dB
 * frequencies for BPF and notch or between
 * midpoint (dBgain/2) gain frequencies for peaking EQ)
 ******************************************************************************/
#define BandWidth             10.0

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#define ORDER           2
#define NUM_OF_FREQ     3
#define NUM_OF_BAND     (NUM_OF_FREQ + 1)

typedef my_float audio_type;
typedef my_float coef_type;
typedef my_float acum_type;

typedef struct sweep_s{
    float               start;
    float               end;
}sweep_t;

union feature{
    float               whole_file;
    sweep_t             sweep;
};

typedef struct cross_prm_s{
    double              gain_dB[NUM_OF_BAND];
    double              freq[NUM_OF_FREQ];
    double              sample_rate;
}cross_prm_t;

typedef struct  apf_coef_t{
    coef_type          k[2];
    coef_type          k_dbl[2];
}apf_coef_t;

typedef struct  coef_t{
    apf_coef_t          apf_1st[NUM_OF_FREQ];
    apf_coef_t          apf_2nd[NUM_OF_FREQ];
    coef_type           gain_c[NUM_OF_BAND];
}coef_t;

typedef struct apf_states_s{
    audio_type          xh[2];
    audio_type          y[2];
    double              xh_dbl[2];
    double              y_dbl[2];
}apf_states_t;

typedef struct cross_states_s{
    apf_states_t    apf_1st[(NUM_OF_FREQ * 2)];
    apf_states_t    apf_2nd[(NUM_OF_FREQ * 2)];
}cross_states_t;

#pragma pack(push,1)
typedef struct chanes_s{
    coef_type Left;
    coef_type Right;
}chanes_t;
#pragma pack(pop)


typedef struct states_t{
    cross_states_t Left;
    cross_states_t Right;
}states_t;

int32_t cross_coeff_calc(cross_prm_t *_prm, coef_t *_coeffs);

#endif