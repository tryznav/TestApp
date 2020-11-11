#include "eq_flt.h"
#include "params_id.h"

int32_t eq_flt_set_prm(eq_prm_t *prm, double samle_rate){
    for(int i = 0; i < 10; i++){
        prm->freq[i] = 100.0 + ((double)i *100.0);
        prm->gain[i] = 3.0;
        prm->Q[i] = 6.0;
        prm->type[i] = PQE;
    }
    prm->sample_rate = samle_rate;
}

int32_t eq_flt_set_state(eq_flt_states_t *st){
    for(int i = 0; i < 10; i++){
        st->x0[i] = 0.0f;
        st->x1[i] = 0.0f;
        st->x2[i] = 0.0f;
        st->y0[i] = 0.0f;
        st->y1[i] = 0.0f;
        st->y2[i] = 0.0f;
    }
}
