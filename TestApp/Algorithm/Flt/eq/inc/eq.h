#ifndef __EQ_H__
#define __EQ_H__

#include "flt_arithmetic.h"
#include "colors.h"
#include "biquad.h"
#include "params_id.h"


#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif


#define ORDER           2
#define NUM_OF_FREQ     3
#define NUM_OF_BAND     (NUM_OF_FREQ + 1)

#define EQ_BAND         10


typedef struct eq_prm_s {
    int32_t         Enable;
    biquad_prm_t    biquad_prm[EQ_BAND];
}eq_prm_t;

typedef struct eq_states_s{
#if CH == 2
    biquad_states_t biquad_states[EQ_BAND];
#elif CH == 8
    vfloat          x1[3];
    vfloat          x2[3];
    vfloat          y1[3];
    vfloat          y2[3];
#endif
}eq_states_t;

typedef struct eq_coef_s{
    int32_t         Enable[10];
#if CH == 2
    biquad_coef_t   biquad_coef[EQ_BAND];
#elif CH == 8
    vfloat        b0[3];
    vfloat        b1[3];
    vfloat        b2[3];
    vfloat        a1[3];
    vfloat        a2[3];
#endif
}eq_coef_t;

void eq_process(vfloat *x, eq_coef_t *coef, eq_states_t *st);

void eq_calculate_coef(eq_prm_t *prm, eq_coef_t *coef);

void eq_set_state(eq_states_t *st);
void eq_set_prm_d(eq_prm_t *prm, double samle_rate);
int32_t eq_set_prm_c(eq_prm_t *prm, id_union_t id_param, float value);


#endif