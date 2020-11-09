#ifndef __APF_FLT_H__
#define __APF_FLT_H__

#include "flt_arithmetic.h"
#include "colors.h"

#include <string.h>
#include <stdio.h>
#include <math.h>

#define TAP_NUM         256
#define APF_ORDER       2
#define APF_2nd         1           

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

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
	double              k[APF_ORDER];
}apf_doub_coefs_t;

typedef struct apf_states_s{
    audio_type          xh[APF_ORDER];
    audio_type          y[APF_ORDER];
    double              xh_dbl[APF_ORDER];
    double              y_dbl[APF_ORDER];
}apf_states_t;

typedef struct apf_coef_s{
    coef_type           k[APF_ORDER];
    double              k_dbl[APF_ORDER];
}apf_coef_t;

#pragma pack(push,1)
typedef struct chanes_s{
    coef_type Left;
    coef_type Right;
}chanes_t;
#pragma pack(pop)

int32_t apf_flt_1st_coef(apf_coef_t *coef, double fc, double sample_rate, int32_t form);
int32_t apf_flt_2nd_coef(apf_coef_t *coef, double fc, double sample_rate, int32_t form);

audio_type apf_flt_1st_DirectI  (audio_type x, apf_states_t *st, apf_coef_t *coef);
audio_type apf_flt_1st_DirectII (audio_type x, apf_states_t *st, apf_coef_t *coef);

audio_type apf_flt_2nd_DirectI  (audio_type x, apf_states_t *st, apf_coef_t *coef);
audio_type apf_flt_2nd_DirectI_t(audio_type x, apf_states_t *st, apf_coef_t *coef);

audio_type apf_flt_2nd_DirectII (audio_type x, apf_states_t *st, apf_coef_t *coef);
audio_type apf_flt_2nd_DirectII_t(audio_type x, apf_states_t *st, apf_coef_t *coef);

audio_type apf_2nd_Lattice      (audio_type x, apf_states_t *st, apf_coef_t *coef);

double apf_dbl_1st_DirectII(double x, apf_states_t *st, apf_coef_t *coef);
double apf_dbl_2nd_DirectI(double x, apf_states_t *st, apf_coef_t *coef);
double apf_dbl_2nd_lattice(double x, apf_states_t *st, apf_coef_t *coef);

#endif