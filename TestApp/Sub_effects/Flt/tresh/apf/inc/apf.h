#ifndef __APF_H__
#define __APF_H__

#include "flt_arithmetic.h"
// #include "params_id.h"
#include "algorithm.h"
// #include "colors.h"

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif


#define ORDER           2
#define NUM_OF_FREQ     3
#define NUM_OF_BAND     (NUM_OF_FREQ + 1)

typedef struct apf_1st_prm_t{
    double          freq;
    double          sample_rate;
    int32_t         Enable;
}apf_1st_prm_t;

typedef struct apf_1st_coef_s{
    int32_t       Enable;
    vfloat        k;
}apf_1st_coef_t;

typedef struct apf_2nd_prm_t{
    double          freq;
    double          Q;
    double          sample_rate;
    int32_t         Enable;
}apf_2nd_prm_t;


typedef struct apf_2nd_coef_s{
    int32_t       Enable;
    vfloat        k1;
    vfloat        k2; 
}apf_2nd_coef_t;

typedef struct apf_1st_states_s{
    vfloat          xh;
}apf_1st_states_t;

typedef struct apf_2nd_states_s{
    vfloat          xh1;
    vfloat          xh2;
}apf_2nd_states_t;


void apf_1st_process(vfloat *x, apf_1st_coef_t *coef,  apf_1st_states_t *st);
void apf_1st_calculate_coef(apf_1st_prm_t *prm,  apf_1st_coef_t *coef);
void apf_1st_set_state(apf_1st_states_t *st);
void apf_1st_set_prm_d(apf_1st_prm_t *prm, double samle_rate);
int32_t apf_1st_set_prm_c(apf_1st_prm_t *prm, id_union_t id_prm, float value);

void apf_2nd_process(vfloat *x, apf_2nd_coef_t *coef,  apf_2nd_states_t *st);
void apf_2nd_calculate_coef(apf_2nd_prm_t *prm,  apf_2nd_coef_t *coef);
void apf_2nd_set_state(apf_2nd_states_t *st);
void apf_2nd_set_prm_d(apf_2nd_prm_t *prm, double samle_rate);
int32_t apf_2nd_set_prm_c(apf_2nd_prm_t *prm, id_union_t id_prm, float value);


#endif