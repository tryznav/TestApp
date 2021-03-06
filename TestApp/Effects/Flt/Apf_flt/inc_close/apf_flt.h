#ifndef __APF_FLT_H__
#define __APF_FLT_H__

#include "apf_flt_control.h"
#include "apf_flt_process.h"
#include "params_id.h"

#include "flt_arithmetic.h"
#include "colors.h"

#include <string.h>
#include <stdio.h>
#include <math.h>

#define TAP_NUM         256
#define APF_ORDER       2
#define APF_2nd         1           

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

typedef struct apf_prm_s{
    int32_t             order;
    double              freq;
    double              fb;
    int32_t             form;
    double              sample_rate;
}apf_prm_t;

typedef struct canon_doub_coefs_s{
	double              c[APF_ORDER];
}apf_doub_coefs_t;

typedef struct apfcoefs_s{
    coef_type           c[APF_ORDER];
    double              c_dbl[APF_ORDER];
    int32_t             order;
    int32_t             form;
}apf_coefs_t;

#pragma pack(push,1)
typedef struct chanes_s{
    coef_type Left;
    coef_type Right;
}chanes_t;
#pragma pack(pop)

typedef struct apf_states_s{
    audio_type          xh[APF_ORDER];
    double              xh_dbl[APF_ORDER];
    audio_type          y[APF_ORDER];
     double             y_dbl[APF_ORDER];
}apf_states_t;

typedef struct states_s{
    apf_states_t    Left;
    apf_states_t    Right;
}states_t;


int32_t apf_coeff_calc(apf_prm_t *_prm, apf_coefs_t *_coeffs);
int32_t apf_settings_write(apf_prm_t *prm);
int32_t apf_settings_read(apf_prm_t *prm);

#endif