#include "eq_fxd.h"

#define ROOM     1
 
audio_type eq_flt(audio_type x, eq_flt_states_t *st,  eq_flt_coef_t *coef){
    audio_type x_tmp = right_shift_q31(x, ROOM);
    acum_type acc = 0;

    for(size_t j = 0; j < 10; j++){   
        if(coef->Enable[j] != 0)
        {  
            st->x0[j]  = x_tmp;
            acc =  0;
            acc =  (acc, st->noise[j]);
            acc =  fxd_mac_m(acc, coef->b0[j], st->x0[j]);
            acc =  fxd_mac_m(acc, coef->b1[j], st->x1[j]);
            acc =  fxd_mac_m(acc, coef->b2[j], st->x2[j]);
            acc = fxd_msub_m(acc, coef->a1[j], st->y1[j]);
            acc = fxd_msub_m(acc, coef->a2[j], st->y2[j]);

            st->noise[j] = fxd_get_high(acc);
            acc = left_shift_q63(acc, NORM);              
            x_tmp = left_shift_q31(gethigh(acc), ROOM);

            st->x2[j] = st->x1[j];
            st->x1[j] = st->x0[j];

            st->y2[j] = st->y1[j];
            st->y1[j] = gethigh(acc);

            // printf("xr = %f\n",x);
            // printf("coef->b1[j] = %f\n",coef->b1[j]);
        }
    }
    return x_tmp;
}
