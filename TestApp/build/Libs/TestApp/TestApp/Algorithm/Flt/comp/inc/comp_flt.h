#ifndef __COMP_FLT_H__
#define __COMP_FLT_H__

#include "colors.h"
#include "flt_arithmetic.h"
#include "params_id.h"

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif


#define ORDER           2
#define NUM_OF_FREQ     3
#define NUM_OF_BAND     (NUM_OF_FREQ + 1)

typedef struct comp_prm_s{
    double threshold;
    double ratio;
    double tauAttack;
    double tauRelease;
    double makeUpGain;
    double samplerate;

    double tauEnvAtt;
    double tauEnvRel;
    int32_t Enable;
    /*float kneeWidth;*/
}comp_prm_t;

typedef struct comp_flt_coef_s{
    vfloat threshold;
    vfloat ratio;

    vfloat a_attack;//alpha attack
    vfloat      a_release;//alpha relise

    vfloat    e_attack; //env attack
    vfloat    e_release; //env relise

    vfloat    makeUpGain;
    int32_t   Enable;
}comp_flt_coef_t;


typedef struct comp_flt_states_s{
    vfloat x;   //
    vfloat y;

    vfloat x_dB; //
    vfloat x_sc;       //

    vfloat g_c;

    vfloat g_s;
    vfloat g_sPrev;

    vfloat g_m;

    vfloat g_lin;

    vfloat env;     //
    vfloat env_prev;
}comp_flt_states_t;

int32_t comp_flt_coef(comp_prm_t *prm, comp_flt_coef_t *coef);
vfloat compressor(vfloat x, comp_flt_states_t *st, comp_flt_coef_t *coef);
int32_t comp_flt_set_state(comp_flt_states_t *st);
int32_t comp_flt_set_prm_d(comp_prm_t*prm, double samle_rate);
int32_t comp_flt_set_prm_c(comp_prm_t* prm, uint8_t Id, float value);

#endif
