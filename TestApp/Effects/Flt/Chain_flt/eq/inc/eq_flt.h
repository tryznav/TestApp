#ifndef __EQ_FLT_H__
#define __EQ_FLT_H__

#include "colors.h"
#include "flt_arithmetic.h"

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif


#define ORDER           2
#define NUM_OF_FREQ     3
#define NUM_OF_BAND     (NUM_OF_FREQ + 1)

typedef my_float audio_type;
typedef my_float coef_type;
typedef my_float acum_type;


typedef struct eq_prm_s {
    double          sample_rate;
    double          freq[10];
    double          gain[10]; 
    double          Q[10];
    int32_t         type[10];
} eq_prm_t;

typedef struct eq_flt_states_s{
    audio_type          x0[10];
    audio_type          x1[10];
    audio_type          x2[10];
    audio_type          y0[10];
    audio_type          y1[10];
    audio_type          y2[10];
} eq_flt_states_t;

typedef struct eq_flt_coef_s{
    int32_t          Enable[10];
    coef_type        b0[10];
    coef_type        b1[10];
    coef_type        b2[10];
    coef_type        a1[10];
    coef_type        a2[10];
}eq_flt_coef_t;


int32_t eq_flt_coef(eq_prm_t *prm,  eq_flt_coef_t *coef);
audio_type eq_flt(audio_type x, eq_flt_states_t *st,  eq_flt_coef_t *coef);

int32_t eq_flt_set_state(eq_flt_states_t *st);
int32_t eq_flt_set_prm(eq_prm_t *prm, double samle_rate);


#endif