#include "cross_fxd.h"

int32_t cross_fxd_set_prm_d(cross_prm_t *prm, double samle_rate){
    prm->freq[0] = 1000;
    prm->freq[1] = 5000;
    prm->freq[2] = 10000;
    prm->sample_rate = samle_rate;
    return 0;
}

int32_t cross_fxd_set_prm_c(cross_prm_t *prm, uint8_t Id_prm, float value){
    switch (Id_prm)
    {
    case 1:
        prm->freq[0] = (double)value;
        break;
    case 2:
        prm->freq[1] = (double)value;
        break;
    case 3:
        prm->freq[2]  = (double)value;
        break;
    default:
        break;
    }
    return 0;
}

int32_t cross_fxd_set_state(cross4b_fxd_states_t *st){
//conmensation state
    st->c_f0.f[0] = 0;
    st->c_f0.f[1] = 0;
    st->c_f0.s[0] = 0;
    st->c_f0.s[1] = 0;
    st->c_f0.s[2] = 0;
    st->c_f0.s[3] = 0;
    st->c_f2.f[0] = 0;
    st->c_f2.f[1] = 0;
    st->c_f2.s[0] = 0;
    st->c_f2.s[1] = 0;
    st->c_f2.s[2] = 0;
    st->c_f2.s[3] = 0;
//first freq
    st->f0.f_1[0] = 0;
    st->f0.f_1[1] = 0;
    st->f0.f_2[0] = 0;
    st->f0.f_2[1] = 0;

    st->f0.f_c[0] = 0;
    st->f0.f_c[1] = 0;

    st->f0.s_1[0] = 0;
    st->f0.s_1[1] = 0;
    st->f0.s_1[2] = 0;
    st->f0.s_1[3] = 0;

    st->f0.s_2[0] = 0;
    st->f0.s_2[1] = 0;
    st->f0.s_2[3] = 0;
    st->f0.s_2[4] = 0;
// freq
    st->f1.f_1[0] = 0;
    st->f1.f_1[1] = 0;
    st->f1.f_2[0] = 0;
    st->f1.f_2[1] = 0;
    st->f1.f_c[0] = 0;
    st->f1.f_c[1] = 0;
    st->f1.s_1[0] = 0;
    st->f1.s_1[1] = 0;
    st->f1.s_1[2] = 0;
    st->f1.s_1[3] = 0;
    st->f1.s_2[0] = 0;
    st->f1.s_2[1] = 0;
    st->f1.s_2[3] = 0;
    st->f1.s_2[4] = 0;
// freq
    st->f2.f_1[0] = 0;
    st->f2.f_1[1] = 0;
    st->f2.f_2[0] = 0;
    st->f2.f_2[1] = 0;
    st->f2.f_c[0] = 0;
    st->f2.f_c[1] = 0;
    st->f2.s_1[0] = 0;
    st->f2.s_1[1] = 0;
    st->f2.s_1[2] = 0;
    st->f2.s_1[3] = 0;
    st->f2.s_2[0] = 0;
    st->f2.s_2[1] = 0;
    st->f2.s_2[3] = 0;
    st->f2.s_2[4] = 0;

    return 0;
}
