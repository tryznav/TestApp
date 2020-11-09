#ifndef __APF_FLT_API_H__
#define __APF_FLT_API_H__

#include "apf_flt_control.h"
#include "apf_flt_process.h"
#include "params_id.h"
#include "colors.h"

#include "apf_flt.h"

typedef struct states_s{
    apf_states_t    Left;
    apf_states_t    Right;
}states_t;

typedef struct coef_s{
    apf_coef_t          apf_coef;
    audio_type          (*apf)(audio_type , apf_states_t* , apf_coef_t *);//      audio_type    (audio_type x, apf_states_t *st, apf_coef_t *coef);
    double              (*apf_dbl)(double , apf_states_t* , apf_coef_t *);//      audio_type    (audio_type x, apf_states_t *st, apf_coef_t *coef);
}coef_t;

#endif
