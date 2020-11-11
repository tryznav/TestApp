#ifndef __CROSS_FLT_H__
#define __CROSS_FLT_H__

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

typedef struct apf_states_1st_s{
    audio_type          xh[2];
}apf_states_1st_t;

typedef struct apf_states_2nd_s{
    audio_type          xh[1];
}apf_states_2nd_t;

typedef struct cross_prm_s{
    double          freq[NUM_OF_FREQ];
    double          sample_rate;
}cross_prm_t;

typedef struct apf_coef_1st_s{
    coef_type           k1;
}apf_coef_1st_t;

typedef struct apf_coef_2nd_s{
    coef_type           k1;
    coef_type           k2;
}apf_coef_2nd_t;

typedef struct  cross_coef_t{
    apf_coef_1st_t      apf_1st[NUM_OF_FREQ];
    apf_coef_2nd_t      apf_2nd[NUM_OF_FREQ];
}cross_coef_t;

typedef struct cross_states_s{
    apf_states_1st_t    apf_1st[(NUM_OF_FREQ * 2)];
    apf_states_2nd_t    apf_2nd[(NUM_OF_FREQ * 2)];
}cross_states_t;

typedef struct bands_s{
    audio_type      band1;
    audio_type      band2;
    audio_type      band3;
    audio_type      band4;
}bands_t;

int32_t cross_flt_coef(cross_prm_t *prm, cross_coef_t *coef);
bands_t crossover_flt(audio_type x, cross_states_t *st, cross_coef_t *coef);

#endif