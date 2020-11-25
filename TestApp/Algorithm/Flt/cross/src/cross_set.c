#include "cross.h"

void cross_set_prm_d(cross_prm_t *prm, double samle_rate){
    prm->freq[0] = 100;
    prm->freq[1] = 600;
    prm->freq[2] = 1000;
    prm->sample_rate = samle_rate;

}

int32_t cross_set_prm_c(cross_prm_t *prm, id_union_t id_prm, float value){
    switch (id_prm.all)
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
#if CH == 2
void cross_set_state(cross4b_states_t *st){
    for(int i = 0; i < CH; i++){
        st->c_f0.xh1.ch[i] = 0.0f;
        st->c_f0.xh2.ch[i] = 0.0f;
        st->c_f2.xh1.ch[i] = 0.0f;
        st->c_f2.xh2.ch[i] = 0.0f;

        st->c_f22.xh1.ch[i] = 0.0f;
        st->c_f22.xh2.ch[i] = 0.0f;

        st->f0.xh.ch[i] = 0.0f;
        st->f0.xh1.ch[i] = 0.0f;
        st->f0.xh2.ch[i] = 0.0f;
        st->f1.xh.ch[i] = 0.0f;
        st->f1.xh1.ch[i] = 0.0f;
        st->f1.xh2.ch[i] = 0.0f;
        st->f2.xh.ch[i] = 0.0f;
        st->f2.xh1.ch[i] = 0.0f;
        st->f2.xh2.ch[i] = 0.0f;
    }
}
#endif

#if CH == 8
void cross_set_state(cross4b_states_t *st){
    for(int i = 0; i < CH; i++){
        st->f0_f2.xh.ch[i] = 0.0f;
        st->f0_f2.xh1.ch[i] = 0.0f;
        st->f0_f2.xh2.ch[i] = 0.0f;
        st->f0_f2.xh1_c.ch[i] = 0.0f;
        st->f0_f2.xh2_c.ch[i] = 0.0f;
        st->f1.xh.ch[i] = 0.0f;
        st->f1.xh1.ch[i] = 0.0f;
        st->f1.xh2.ch[i] = 0.0f;
    }
}
#endif
