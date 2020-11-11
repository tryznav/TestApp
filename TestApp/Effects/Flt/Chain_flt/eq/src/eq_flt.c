#include "eq_flt.h"
 
audio_type eq_flt(audio_type x, eq_flt_states_t *st,  eq_flt_coef_t *coef){
    for(size_t j = 0; j < 10; j++){   
        if(coef->Enable[j] != 0)
        {

            st->x0[j] = x;

            st->y0[j] = flt_mac(coef->b0[j], st->x0[j], st->x1[j]) * 8;

            st->x1[j] = flt_mac(coef->b1[j], st->x0[j], st->x2[j]);
            st->x1[j] = flt_msub(coef->a1[j], st->y0[j], st->x1[j]);

            st->x2[j] = flt_mul(coef->b2[j], st->x0[j]);
            st->x2[j] = flt_msub(coef->a2[j], st->y0[j], st->x2[j]);

            x = st->y0[j];
        }
    }
    return x;
}
