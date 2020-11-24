#include "eq_fxd.h"

#define ROOM        1
#define NORM        3
#define COEF_FR      31
 
audio_type eq_fxd(audio_type x, eq_fxd_states_t *st,  eq_fxd_coef_t *coef){
    acum_type acc = 0;
    x = fxd_rshift(x, ROOM);

    for(size_t j = 0; j < 10; j++){   
        if(coef->Enable[j] != 0)
        {
            acc =  0;
            acc =  (acc, st->noise[j]);
            acc =  fxd_mac_m(acc, coef->b0[j], x);
            acc =  fxd_mac_m(acc, coef->b1[j], st->x1[j]);
            acc =  fxd_mac_m(acc, coef->b2[j], st->x2[j]);
            acc = fxd_msub_m(acc, coef->a1[j], st->y1[j]);
            acc = fxd_msub_m(acc, coef->a2[j], st->y2[j]);

            st->noise[j] = (fxd_q31_t)(acc & ((1u << (COEF_FR - NORM)) - 1));

            st->x2[j] = st->x1[j];
            st->x1[j] = x;

            st->y2[j] = st->y1[j];
            st->y1[j] = (audio_type)saturation(fxd63_rshift(acc, (COEF_FR - NORM)));
            x = st->y1[j];
        }
    }
    x = (audio_type)saturation(fxd63_rshift(acc,  (COEF_FR - NORM - ROOM)));
    return x;
}
