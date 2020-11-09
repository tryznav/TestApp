#ifndef __CROSS_FLT_H__
#define __CROSS_FLT_H__

#include "apf_flt.h"
#include "colors.h"

#define NUM_OF_FREQ     3
#define NUM_OF_BAND     (NUM_OF_FREQ + 1)

typedef struct cross_prm_s{
    double          freq[NUM_OF_FREQ];
    double          sample_rate;
    int32_t         apf_form1;
    int32_t         apf_form2;
}cross_prm_t;

typedef struct  cross_coef_t{
    apf_coef_t          apf_1st[NUM_OF_FREQ];
    apf_coef_t          apf_2nd[NUM_OF_FREQ];
    audio_type          (*apf_1st_f)(audio_type , apf_states_t* , apf_coef_t *);//      audio_type    (audio_type x, apf_states_t *st, apf_coef_t *coef);
    audio_type          (*apf_2nd_f)(audio_type , apf_states_t* , apf_coef_t *);//      audio_type    (audio_type x, apf_states_t *st, apf_coef_t *coef);
    double              (*apf_1st_f_dbl)(double , apf_states_t* , apf_coef_t *);//      audio_type    (audio_type x, apf_states_t *st, apf_coef_t *coef);
    double              (*apf_2nd_f_dbl)(double , apf_states_t* , apf_coef_t *);//      audio_type    (audio_type x, apf_states_t *st, apf_coef_t *coef);
}cross_coef_t;

typedef struct cross_states_s{
    apf_states_t    apf_1st[(NUM_OF_FREQ * 2)];
    apf_states_t    apf_2nd[(NUM_OF_FREQ * 2)];
}cross_states_t;

typedef struct bands_s{
    audio_type      band1;
    audio_type      band2;
    audio_type      band3;
    audio_type      band4;
}bands_t;

typedef struct bands_dbl_s{
    double          band1;
    double          band2;
    double          band3;
    double          band4;
}bands_dbl_t;

int32_t cross_flt_coef(cross_prm_t *prm, cross_coef_t *coef);
bands_t crossover_flt(audio_type x, cross_states_t *st, cross_coef_t *coef);
bands_dbl_t crossover_dbl(double x, cross_states_t *st, cross_coef_t *coef);

#endif