#include "eq_flt.h"
#include "params_id.h"

int32_t eq_flt_set_prm_d(eq_prm_t *prm, double samle_rate){
    for(int i = 0; i < 10; i++){
        prm->freq[i] = 100.0 + ((double)i *500.0);
        prm->gain[i] = 3.0;
        prm->Q[i] = 6.0;
        prm->type[i] = PQE;
        prm->Enable[i] = 1;
    }

    // prm->freq[0] = 1000.0;
    // prm->gain[0] = 0;
    // prm->Q[0] = 1.0;
    // prm->type[0] = HPF;
    prm->sample_rate = samle_rate;
    return 0;
}

int32_t eq_flt_set_prm_c(eq_prm_t *prm, uint8_t Id_f , uint8_t Id_prm, float value){
    switch (Id_prm)
    {
    case 1:
        prm->freq[Id_f] = (double)value;
        break;
    case 2:
       prm->gain[Id_f] = (double)value;
        break;
    case 3:
        prm->Q[Id_f] = (double)value;
        break;
    case 4:
        prm->type[Id_f] = (int32_t)value;
        break;
    case Enable_id:
        prm->Enable[Id_f] = (int32_t)value;
        break;
    default:
        break;
    }
    return 0;
}

int32_t eq_flt_set_state(eq_flt_states_t *st){
    for(int i = 0; i < 10; i++){
        st->x1[i] = 0.0f;
        st->x2[i] = 0.0f;
        st->y1[i] = 0.0f;
        st->y2[i] = 0.0f;
    }
    return 0;
}
