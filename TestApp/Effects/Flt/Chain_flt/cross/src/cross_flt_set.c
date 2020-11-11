#include "cross_flt.h"

int32_t cross_flt_set_prm(cross_prm_t *prm, double samle_rate){
    prm->freq[0] = 200;
    prm->freq[0] = 1000;
    prm->freq[0] = 10000;
    prm->sample_rate = samle_rate;
}

int32_t cross_flt_set_state(cross_flt_states_t *st){
    st->apf_1st->xh[0] = 0.0f;
    st->apf_1st->xh[1] = 0.0f;
    st->apf_2nd->xh[0] = 0.0f;
    st->apf_2nd->xh[1] = 0.0f;
}
