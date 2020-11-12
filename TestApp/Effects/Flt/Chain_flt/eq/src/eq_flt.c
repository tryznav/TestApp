#include "eq_flt.h"
 
audio_type eq_flt(audio_type x, eq_flt_states_t *st,  eq_flt_coef_t *coef){
    audio_type xr = x;

    for(size_t j = 0; j < 10; j++){   
        if(coef->Enable[j] != 0)
        {
            st->x0[j] = xr;

            st->y0[j] = flt_mac(st->x1[j], coef->b0[j], st->x0[j]);

            st->x1[j] = flt_mac(st->x2[j], coef->b1[j], st->x0[j]);
            st->x1[j] = flt_msub(st->x1[j], coef->a1[j], st->y0[j]);

            st->x2[j] = flt_mul(coef->b2[j], st->x0[j]);
            st->x2[j] = flt_msub(st->x2[j], coef->a2[j], st->y0[j]);

            xr = st->y0[j];

            // printf("xr = %f\n",x);
            // printf("coef->b1[j] = %f\n",coef->b1[j]);
        }
    }
    return xr;
}
