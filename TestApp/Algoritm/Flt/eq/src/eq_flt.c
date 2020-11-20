#include "eq_flt.h"
 
audio_type eq_flt(audio_type x, eq_flt_states_t *st,  eq_flt_coef_t *coef){
    audio_type y = 0.0f;

    for(size_t j = 0; j < 10; j++){   
        if(coef->Enable[j] != 0)
        {
            y = flt_mac(st->x1[j], coef->b0[j], x);

            st->x1[j] = flt_mac(st->x2[j], coef->b1[j], x);
            st->x1[j] = flt_msub(st->x1[j], coef->a1[j], y);

            st->x2[j] = flt_mul(coef->b2[j], x);
            st->x2[j] = flt_msub(st->x2[j], coef->a2[j], y);

            x = y;
        }
    }
    return x;
}
