#include "cross_flt.h"

int32_t cross_flt_set_prm(cross_prm_t *prm, double samle_rate){
    prm->freq[0] = 200;
    prm->freq[1] = 1000;
    prm->freq[2] = 10000;
    prm->sample_rate = samle_rate;
    return 0;
}

int32_t cross_flt_set_state(cross_flt_states_t *st){
    for(int i = 0; i < 15; i++){
        st->apf_1st[i].xh[0] = 0.0f;
        // st->apf_1st->xh[1] = 0.0f;
        st->apf_2nd[i].xh[0] = 0.0f;
        st->apf_2nd[i].xh[1] = 0.0f;
    }
    return 0;
}
