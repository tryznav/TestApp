#include "comp_flt.h"

int32_t comp_flt_set_prm(comp_prm_t*prm, double samle_rate){
    prm->threshold = -18.0;
    prm->ratio = 40.0;
    prm->tauAttack = 30.0;
    prm->tauRelease = 100.0;
    prm->makeUpGain = 0.0;
    prm->samplerate = samle_rate;
    return 0;
}

int32_t comp_flt_set_state(comp_flt_states_t *st){
    st->x = 0.0f;   //
    st->y = 0.0f;
    st->x_dB = 0.0f; //
    st->x_sc = 0.0f;       //
    st->g_c = 0.0f;
    st->g_s = 0.0f;
    st->g_sPrev = 0.0f;
    st->g_m = 0.0f;
    st->g_lin = 0.0f;
    st->env = 0.0f;     //
    st->env_prev = 0.0f;
    return 0;
}
