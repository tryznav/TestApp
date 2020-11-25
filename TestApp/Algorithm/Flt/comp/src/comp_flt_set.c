#include "comp_flt.h"

int32_t comp_set_prm_d(comp_prm_t* prm, double samle_rate){
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

int32_t comp_set_prm_c(comp_prm_t* prm, uint8_t Id, float value){
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

int32_t comp_set_state(comp_states_t *st){
    for(int i = 0; i < CH; i++){
        st->x.ch[i] = 0.0f;   //
        st->y.ch[i] = 0.0f;
        st->x_dB.ch[i] = 0.0f; //
        st->x_sc.ch[i] = 0.0f;       //
        st->g_c.ch[i] = 0.0f;
        st->g_s.ch[i] = 0.0f;
        st->g_sPrev.ch[i] = 0.0f;
        st->g_m.ch[i] = 0.0f;
        st->g_lin.ch[i] = 0.0f;
        st->env.ch[i] = 0.0f;     //
        st->env_prev.ch[i] = 0.0f;
    }

    return 0;
}
