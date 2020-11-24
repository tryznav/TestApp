#ifndef __COMP_FXD_H__
#define __COMP_FXD_H__

#include "colors.h"
#include "fxd_arithmetic.h"
#include "params_id.h"

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif


#define ORDER           2
#define NUM_OF_FREQ     3
#define NUM_OF_BAND     (NUM_OF_FREQ + 1)

#define GAIN_FR         (31-4)

typedef fxd_q31_t audio_type;
typedef fxd_q31_t coef_type;
typedef fxd_q63_t acum_type;

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

typedef struct comp_fxd_coef_s{
    coef_type threshold;
    coef_type ratio;

    coef_type a_attack;//alpha attack
    coef_type a_release;//alpha relise

    coef_type e_attack; //env attack
    coef_type e_release; //env relise

    coef_type makeUpGain;
    int32_t   Enable;
}comp_fxd_coef_t;


typedef struct comp_fxd_states_s{
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
}comp_fxd_states_t;

int32_t comp_fxd_coef(comp_prm_t *prm, comp_fxd_coef_t *coef);
audio_type compressor_fxd(audio_type x, comp_fxd_states_t *st, comp_fxd_coef_t *coef);
int32_t comp_fxd_set_state(comp_fxd_states_t *st);
int32_t comp_fxd_set_prm_d(comp_prm_t*prm, double samle_rate);
int32_t comp_fxd_set_prm_c(comp_prm_t* prm, uint8_t Id, float value);

#endif
