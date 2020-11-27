#include "biquad.h"
#include "params_id.h"

void biquad_cascade_set_prm_d(biquad_prm_t *prm, double samle_rate){
    for(int i = 0; i < 10; i++){
        prm->freq = 100.0;
        prm->gain = 3.0;
        prm->Q = 6.0;
        prm->type = PQE;
        prm->Enable = 1;
    }

    prm->sample_rate = samle_rate;
}

int32_t biquad_cascade_set_prm_c(biquad_prm_t *prm, id_union_t Id_prm, float value){
    switch (Id_prm.id.prm)
    {
    case 1:
        prm->freq = (double)value;
        break;
    case 2:
       prm->gain = (double)value;
        break;
    case 3:
        prm->Q = (double)value;
        break;
    case 4:
        prm->type = (int32_t)value;
        break;
    case Enable_id:
        prm->Enable = (int32_t)value;
        break;
    default:
        break;
    }
    return 0;
}

void biquad_set_state(biquad_states_t *st){
    for(int i = 0; i < CH; i++){
        st->x1.ch[i] = 0.0f;
        st->x2.ch[i] = 0.0f;
        st->y1.ch[i] = 0.0f;
        st->y2.ch[i] = 0.0f;
    }
}
