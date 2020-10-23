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

// typedef struct cross_doub_coefs_s{
// 	double              c_1st;
// 	double              c_2nd;
// 	double              d_2nd;
// 	double              b0;
// 	double              b1;
// 	double              b2;
// }cross_doub_coefs_t;

typedef struct canon_doub_coefs_s{
	double              a1;
	double              a2;
	double              b0;
	double              b1;
	double              b2;
}canon_doub_coefs_t;

// typedef struct cross_coefs_s{
// 	coef_type           c_1st;
// 	coef_type           c_2nd;
// 	coef_type           d_2nd;
// 	coef_type           b0;
// 	coef_type           b1;
// 	coef_type           b2;
// }cross_coefs_t;

typedef struct canon_coefs_s{
	coef_type           a1;
	coef_type           a2;
	coef_type           b0;
	coef_type           b1;
	coef_type           b2;
}canon_coefs_t;

typedef struct apf_coef_1st_s{
	coef_type           c;
}apf_coef_1st_t;

typedef struct all_pass_2nd_s{
    coef_type           c;
	coef_type           dc;                 // d(1 - c)
}apf_coef_2nd_t;

typedef struct cross_coefs_s{
	canon_coefs_t       hpf[NUM_OF_FREQ];
    canon_coefs_t       lpf[NUM_OF_FREQ];
    apf_coef_1st_t      apf_1st[NUM_OF_FREQ];
    apf_coef_2nd_t      apf_2nd[NUM_OF_FREQ];
    coef_type           gain_p[NUM_OF_BAND];        
}cross_coefs_t;

#pragma pack(push,1)
typedef struct chanes_s{
    coef_type Left;
    coef_type Right;
}chanes_t;
#pragma pack(pop)


typedef struct xh_z_s{
    audio_type z_1;
    audio_type z_2;
}xh_z_t;


typedef struct cross_s{
    xh_z_t          xh_lpf[NUM_OF_FREQ];
    xh_z_t          xh_hpf[NUM_OF_FREQ];
    xh_z_t          xh_apf_2ord_h[NUM_OF_FREQ];
    xh_z_t          xh_apf_2ord_ll[NUM_OF_FREQ];
    xh_z_t          xh_apf_2ord_lh[NUM_OF_FREQ];
    xh_z_t          xh_apf_2ord[NUM_OF_FREQ];
    audio_type      xh_apf_1ord[NUM_OF_FREQ];
}cross_t;

typedef struct cross_states_t{
    cross_t Left;
    cross_t Right;
}cross_states_t;

int32_t cross_coeff_calc(cross_prm_t *_prm, cross_coefs_t *_coeffs);

#endif