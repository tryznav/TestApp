#ifndef __expan_FLT_H__
#define __expan_FLT_H__

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

typedef struct expan_prm_s{
    double threshold;
    double ratio;
    double tauAttack;
    double tauRelease;

    double makeUpGain;
    double samplerate;

    double tauEnvAtt;
    double tauEnvRel;
    /*float kneeWidth;*/
}expan_prm_t;

typedef struct expan_flt_coef_s{
    coef_type threshold;
    coef_type ratio;

    coef_type a_attack;     //alpha attack
    coef_type a_release;    //alpha relise

    coef_type e_attack;     //env attack
    coef_type e_release;    //env relise

    coef_type makeUpGain;
    coef_type samplerate;
}expan_flt_coef_t;


typedef struct expan_flt_states_s{
    audio_type x;   //
    audio_type y;

    audio_type x_dB; //
    audio_type x_sc;       //

    audio_type g_c;

    audio_type g_s;
    audio_type g_sPrev;

    audio_type g_m;

    audio_type g_lin;

    audio_type env;     //
    audio_type env_prev;
}expan_flt_states_t;

int32_t expan_flt_coef(expan_prm_t *prm, expan_flt_coef_t *coef);
audio_type expander_flt(audio_type x, expan_flt_states_t *st, expan_flt_coef_t *coef);
int32_t expan_flt_set_state(expan_flt_states_t *st);
int32_t expan_flt_set_prm(expan_prm_t*prm, double samle_rate);

#endif