#ifndef __BIQUAD_H__
#define __BIQUAD_H__

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


typedef struct biquad_prm_s {
    double          sample_rate;
    double          freq;
    double          gain; 
    double          Q;
    int32_t         type;
    int32_t         Enable;
}biquad_prm_t;

typedef struct biquad_states_s{
    vfloat          x1;
    vfloat          x2;
    vfloat          y1;
    vfloat          y2;
}biquad_states_t;

typedef struct biquad_coef_s{
    int32_t       Enable;
    vfloat        b0;
    vfloat        b1;
    vfloat        b2;
    vfloat        a1;
    vfloat        a2;
}biquad_coef_t;


static inline void biquad_process(vfloat *x, biquad_coef_t *coef,  biquad_states_t *st);

void biquad_calculate_coef(biquad_prm_t *prm,  biquad_coef_t *coef);
void biquad_set_state(biquad_states_t *st);
void biquad_set_prm_d(biquad_prm_t *prm, double samle_rate);
int32_t biquad_set_prm_c(biquad_prm_t *prm, id_union_t id_prm, float value);


#endif