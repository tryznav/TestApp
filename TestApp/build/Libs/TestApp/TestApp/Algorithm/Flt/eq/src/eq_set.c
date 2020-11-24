#include "eq.h"
#include "params_id.h"

void eq_set_prm_d(eq_prm_t *prm, double samle_rate){
    for(int i = 0; i < EQ_BAND; i++){
        biquad_set_prm_d(&prm->biquad_prm[i], samle_rate);
        prm->biquad_prm[i].freq = 100.0 + ((double)i *500.0);
    }
}

int32_t eq_set_prm_c(eq_prm_t *prm, id_union_t id_prm, float value){
    if(id_prm.id.effect > EQ_BAND && id_prm.id.effect > 255){
        fprintf(stderr, RED"Error: "BOLDWHITE"Invalid EQ SUB effect ID. Reject\n"RESET);
        return -1;
    }
    if(id_prm.id.effect == 255){
        prm->Enable = (int32_t)value;
    } else {
        biquad_set_prm_c(&prm->biquad_prm[id_prm.id.sub_effect], id_prm, value);
    }
    return 0;
}

void eq_set_state(eq_states_t *st){
    for(int i = 0; i < EQ_BAND; i++){
        biquad_set_state(&st->biquad_states[i]);
    }
}
