#include "comp_fxd.h"

int32_t comp_fxd_set_prm_d(comp_prm_t* prm, double samle_rate){
    prm->threshold = -18.0;
    prm->ratio = 2.0;
    prm->tauAttack = 20.0;
    prm->tauRelease = 40.0;
    prm->tauEnvAtt = 0.00001;
    prm->tauEnvRel = 30;
    prm->makeUpGain = 0.0;
    prm->Enable = 1;
    prm->samplerate = samle_rate;
    return 0;
}

int32_t comp_fxd_set_prm_c(comp_prm_t* prm, uint8_t Id, float value){
    switch (Id)
    {
    case 1:
        prm->threshold = (double)value;
        break;
    case 2:
        prm->ratio = (double)value;
        break;
    case 3:
        prm->tauAttack = (double)value;
        break;
    case 4:
        prm->tauRelease = (double)value;
        break;
    case 5:
        prm->tauEnvAtt = (double)value;
        break;
    case 6:
        prm->tauEnvRel = (double)value;
        break;
    case 7:
        prm->makeUpGain = (double)value;
        break;
    case Enable_id:
        prm->Enable = (int32_t)value;
        break;
    default:
        break;
    }
    return 0;
}

int32_t comp_fxd_set_state(comp_fxd_states_t *st){
    st->x = 0;   //
    st->y = 0;
    st->x_dB = 0; //
    st->x_sc = 0;       //
    st->g_c = 0;
    st->g_s = 0;
    st->g_sPrev = 0;
    st->g_m = 0;
    st->g_lin = 0;
    st->env = 0;     //
    st->env_prev = 0;
    return 0;
}
